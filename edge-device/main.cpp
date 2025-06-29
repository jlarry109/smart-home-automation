#include "ConfigLoader.hpp"
#include "motion/MotionMonitor.hpp"
#include "motion/MockMotionSensor.hpp"
#include "mqtt/PahoMqttClient.hpp"
#include <memory>
#include <thread>
#include <chrono>
#include <iostream>

int main() {
    MqttConfig config = ConfigLoader::load("config.json");

    std::shared_ptr<IMotionSensor> mockSensor = std::make_shared<MockMotionSensor>();
    std::shared_ptr<IMqttClient> mqttClient = std::shared_ptr<IMqttClient>(
            new PahoMqttClient(
                    config.serverURI,
                    config.clientId,
                    config.certPath,
                    config.keyPath,
                    config.caPath
            )
    );

    MotionMonitor monitor(mockSensor, mqttClient);

    try {
        mqttClient->connect();
        monitor.startMonitoring(5000); // every 5 seconds

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
