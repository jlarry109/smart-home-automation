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
//        mqttClient_->publish("sensors/lux", std::to_string(lux));
        mqttClient_->publishAsync("sensors/lux", std::to_string(lux), 1);
    }
}