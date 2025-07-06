#include "ConfigLoader.hpp"
#include "PahoMqttClient.hpp"

#include "motion/MockMotionSensor.hpp"
#include "temperature_humidity/MockTemperatureHumiditySensor.hpp"
#include "light/MockLightSensor.hpp"
#include "light/LightController.hpp"

#include "rules/SmartRuleEngine.hpp"
#include "rules/MotionAtNightRule.hpp"
#include "rules/HighTempRule.hpp"
#include "rules/HumiditySpikeRule.hpp"

#include "orchestration/SmartMqttOrchestrator.hpp"

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

        // Light control
        auto lightController = std::make_shared<LightController>();

        // Define rule actions
        auto highTempAction = [mqttClient]() {
            std::string msg = "[HighTempRule] 🔥 Overheat detected!";
            threadSafeLog(msg);
            mqttClient->publish("alerts/temperature", msg);
        };

        auto humidityAction = [mqttClient]() {
            std::string msg = "[HumiditySpikeRule] 💧 Humidity spike detected!";
            threadSafeLog(msg);
            mqttClient->publish("alerts/humidity", msg);
        };

        // Register rules
        auto ruleEngine = std::make_shared<SmartRuleEngine>();
        ruleEngine->addRule(std::make_shared<MotionAtNightRule>(lightController));
        ruleEngine->addRule(std::make_shared<HighTempRule>(highTempAction));
        ruleEngine->addRule(std::make_shared<HumiditySpikeRule>(humidityAction));

        // Start orchestrator
        SmartMqttOrchestrator orchestrator(mqttClient, ruleEngine);
        orchestrator.start();

        // Run forever
        while (true) {
            std::this_thread::sleep_for(std::chrono::seconds(60));
        }

    } catch (const mqtt::exception& e) {
        std::cerr << "MQTT Error: " << e.what() << std::endl;
        return 1;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
