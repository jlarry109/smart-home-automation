#include "ConfigLoader.hpp"
#include "motion/MotionMonitor.hpp"
#include "motion/MockMotionSensor.hpp"
#include "temperature_humidity/EnvironmentMonitor.hpp"
#include "temperature_humidity/MockTemperatureHumiditySensor.hpp"
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
        MotionMonitor motionMonitor(motionSensor, mqttClient);
        std::thread motionThread([&]() {
            motionMonitor.startMonitoring(5000); // every 5 seconds
        });

        // Temperature & Humidity setup
        auto envSensor = std::make_shared<MockTemperatureHumiditySensor>();
        EnvironmentMonitor envMonitor(envSensor, mqttClient);
        envMonitor.startMonitoring(7000); // every 7 seconds

        // Run forever
        while (true) {
            std::this_thread::sleep_for(std::chrono::seconds(60));
        }

        // Clean shutdown (won't be reached in this version)
        motionThread.join();
        envMonitor.stopMonitoring();

    } catch (const mqtt::exception& e) {
        std::cerr << "MQTT Error: " << e.what() << std::endl;
        return 1;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
