#include "EnvironmentMonitor.hpp"
#include "../../utils/Logging.hpp"
#include <iostream>
#include <chrono>


EnvironmentMonitor::EnvironmentMonitor(std::shared_ptr<ITemperatureHumiditySensor> sensor,
                                       std::shared_ptr <IMqttClient> mqttClient)
                                       : sensor_(std::move(sensor)), mqttClient_(std::move(mqttClient)) {}

EnvironmentMonitor::~EnvironmentMonitor() {
    threadSafeLog("[EnvironmentMonitor] Destructor called.");
    stopMonitoring();
}

void EnvironmentMonitor::startMonitoring(int intervalMs) {
    if (running_) {
        threadSafeLog("[EnvironmentMonitor] Already running");
        return;
    }
    running_ = true;
    workerThread_ = std::thread(&EnvironmentMonitor::monitoringLoop, this, intervalMs);
}
void EnvironmentMonitor::stopMonitoring() {
    if (!running_) {
        threadSafeLog("[EnvironmentMonitor] Already not running");
        return;
    }
    running_ = false;
    if (workerThread_.joinable()) {
        workerThread_.join();
    }
}

void EnvironmentMonitor::monitoringLoop(int intervalMs) {
    try {
        while (running_) {
            auto reading = sensor_->read();
            std::ostringstream oss;
            oss << "[EnvironmentMonitor] 🌡️ Temp: " << reading.temperatureCelsius
                << "°C, 💧 Humidity: " << reading.humidityPercent << "%";
            threadSafeLog(oss.str());

            if (mqttClient_) {
                // Publish temperature and humidity as JSON payloads (or separate topics) => separate topics for now
                std::string tempPayload = std::to_string(reading.temperatureCelsius);
                std::string humidPayload = std::to_string(reading.humidityPercent);

                mqttClient_->publish("environment/temperature", tempPayload);
                mqttClient_->publish("environment/humidity", humidPayload);
            }
        }
        threadSafeLog("[EnvironmentMonitor] Monitoring stopped gracefully.");
    } catch (const std::exception &e) {
        std::cerr << "[EnvironmentMonitor] ERROR reading sensor or publishing to MQTT: " << e.what() << std::endl;
    } catch (...) {
        std::cerr << "[EnvironmentMonitor] Unknown error reading sensor or publishing to MQTT." << std::endl;
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(intervalMs));
}
