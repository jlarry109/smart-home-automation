#include "LuxLogger.hpp"
#include "../../utils/Logging.hpp"
#include <iostream>
#include <cmath>

LuxLogger::LuxLogger(std::shared_ptr<IMqttClient> mqttClient)
    : mqttClient_(std::move(mqttClient)) {}


void LuxLogger::log(float lux) {
    std::ostringstream oss;
    oss << "[LuxLogger] 🌞 Lux: " << lux;
    threadSafeLog(oss.str());

    if (mqttClient_) {
        mqttClient_->publish("lux", std::to_string(lux));
    }
}

void LuxLogger::maybeAlert(float lux) {
    if (lastLux_ >= 0.0f && std::abs(lastLux_ - lux) > 400.0f) {
        std::string msg = "[LuxLogger] ⚠️ Sudden lux change detected: " + std::to_string(lux);
        std::ostringstream oss;
        oss << msg;
        threadSafeLog(oss.str());

        if (mqttClient_) {
            mqttClient_->publish("light/alert", msg);
        }
    }
    lastLux_ = lux;
}