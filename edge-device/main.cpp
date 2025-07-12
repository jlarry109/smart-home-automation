#include "ConfigLoader.hpp"
#include "PahoMqttClient.hpp"

#include "motion/MockMotionSensor.hpp"
#include "motion/MotionMonitor.hpp"
#include "temperature_humidity/MockTemperatureHumiditySensor.hpp"
#include "temperature_humidity/EnvironmentMonitor.hpp"
#include "light/MockLightSensor.hpp"
#include "light/LightController.hpp"
#include "light/LightMonitor.hpp"

#include "rules/SmartRuleEngine.hpp"
#include "rules/MotionAtNightRule.hpp"
#include "rules/HighTempRule.hpp"
#include "rules/HumiditySpikeRule.hpp"

#include "orchestration/SmartMqttOrchestrator.hpp"
#include "../utils/TimeUtils.hpp"

#include <memory>
#include <chrono>
#include <thread>
#include <iostream>

int main() {
    try {
        // Load config
        MqttConfig config = ConfigLoader::load("config.json");

        // Initialize MQTT client
        auto mqttClient = std::make_shared<PahoMqttClient>(
                config.serverURI,
                config.clientId,
                config.certPath,
                config.keyPath,
                config.caPath
        );
        mqttClient->connect();

        // Define rule actions

        auto highTempAction = [mqttClient](float temp) {
            std::string msg = "[HighTempRule]: Temperature exceeded threshold!";
            nlohmann::json payloadJson = {
                    {"device_id", mqttClient->getClientId()},
                    {"timestamp", getCurrentIsoTimestampUTC()},
                    {"sensor", "temperature"},
                    {"value", temp},
                    {"rule", "HighTempRule"},
                    {"message", msg}
            };
            std::string payload = payloadJson.dump();
            threadSafeLog("[HighTempRule] 🔥 Overheat detected!");
            mqttClient->publish("alerts/temperature", payload);
            threadSafeLog("[HighTempRule] 🔥 Published alert: " + payload);
        };

        auto humidityAction = [mqttClient](float humidity) {
            std::string msg = "[HumiditySpikeRule]: Humidity exceeded threshold!";
            nlohmann::json payloadJson = {
                    {"device_id", mqttClient->getClientId()},
                    {"timestamp", getCurrentIsoTimestampUTC()},
                    {"sensor", "humidity"},
                    {"value", humidity},
                    {"rule", "HumiditySpikeRule"},
                    {"message", msg}
            };
            std::string payload = payloadJson.dump();
            threadSafeLog("[HumiditySpikeRule] 💧 Humidity spike detected!");
            mqttClient->publish("alerts/humidity", payload);
            threadSafeLog("[HumiditySpikeRule] 💧 Published alert: " + payload);
        };

        // Create mock sensors
        auto mockMotionSensor = std::make_shared<MockMotionSensor>();
        auto mockLightSensor = std::make_shared<MockLightSensor>();
        auto mockTempHumidSensor = std::make_shared<MockTemperatureHumiditySensor>();

        auto luxLogger = std::make_shared<LuxLogger>(mqttClient);
        auto lightController = std::make_shared<LightController>();

        // Register rules
        auto ruleEngine = std::make_shared<SmartRuleEngine>();
        ruleEngine->addRule(std::make_shared<MotionAtNightRule>(lightController, mqttClient, 28.0f));
        ruleEngine->addRule(std::make_shared<HighTempRule>(highTempAction, 25.0f));
        ruleEngine->addRule(std::make_shared<HumiditySpikeRule>(humidityAction, 55.0f));

        // Create monitors and pass mqttClient to them
        auto motionMonitor = std::make_shared<MotionMonitor>(mockMotionSensor, mqttClient);
        auto lightMonitor = std::make_shared<LightMonitor>(mockLightSensor, luxLogger, lightController);
        auto envMonitor = std::make_shared<EnvironmentMonitor>(mockTempHumidSensor, mqttClient);

        // Start the monitors to publish sensor data periodically
        motionMonitor->startMonitoring();
        lightMonitor->startMonitoring();
        envMonitor->startMonitoring();

        // Start orchestrator
        SmartMqttOrchestrator orchestrator(mqttClient, ruleEngine);
        orchestrator.start();

        // Run forever
        while (true) {
            std::this_thread::sleep_for(std::chrono::seconds(60));
        }

        // Not reachable now
        motionMonitor->stopMonitoring();
        lightMonitor->stopMonitoring();
        envMonitor->stopMonitoring();
        orchestrator.stop();
        mqttClient->disconnect();

    } catch (const mqtt::exception& e) {
        std::cerr << "MQTT Error: " << e.what() << std::endl;
        return 1;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
