#include "ConfigLoader.hpp"
#include "motion/MotionMonitor.hpp"
#include "motion/MockMotionSensor.hpp"
#include "temperature_humidity/EnvironmentMonitor.hpp"
#include "temperature_humidity/MockTemperatureHumiditySensor.hpp"
#include "light/MockLightSensor.hpp"
#include "light/LuxLogger.hpp"
#include "light/LightController.hpp"
#include "light/LightMonitor.hpp"

#include "PahoMqttClient.hpp"

#include <memory>
#include <thread>
#include <chrono>
#include <iostream>

int main() {
    try {
        MqttConfig config = ConfigLoader::load("config.json");

        auto mqttClient = std::make_shared<PahoMqttClient>(
                config.serverURI,
                config.clientId,
                config.certPath,
                config.keyPath,
                config.caPath
        );

        mqttClient->connect();

        // Motion setup
        auto motionSensor = std::make_shared<MockMotionSensor>();
//        MotionMonitor motionMonitor(motionSensor, mqttClient);
        auto motionMonitor = std::make_shared<MotionMonitor>(motionSensor, mqttClient);
        motionMonitor->startMonitoring(5000); // every 5 seconds

        // Temperature & Humidity setup
        auto envSensor = std::make_shared<MockTemperatureHumiditySensor>();
        auto envMonitor = std::make_shared<EnvironmentMonitor>(envSensor, mqttClient);
        envMonitor->startMonitoring(7000); // every 7 seconds

        // Light Sensor setup
        auto lightSensor = std::make_shared<MockLightSensor>();
        auto lightLogger = std::make_shared<LuxLogger>(mqttClient);
        auto lightController = std::make_shared<LightController>();
        auto lightMonitor = std::make_shared<LightMonitor>(lightSensor, lightLogger, lightController);

        lightMonitor->startMonitoring(10000); // every 10 seconds


        // Run forever
        while (true) {
            std::this_thread::sleep_for(std::chrono::seconds(60));
        }

        // Clean shutdown (won't be reached in this version)
        motionMonitor->stopMonitoring();
        envMonitor->stopMonitoring();
        lightMonitor->stopMonitoring();

    } catch (const mqtt::exception& e) {
        std::cerr << "MQTT Error: " << e.what() << std::endl;
        return 1;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
