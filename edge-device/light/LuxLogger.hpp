#ifndef LUX_LOGGER_HPP
#define LUX_LOGGER_HPP

#include "IMqttClient.hpp"
#include <memory>
#include <string>

/*
 * Publishes lux value and any alerts to MQTT
 * */
class LuxLogger {
public:
    explicit LuxLogger(std::shared_ptr<IMqttClient> mqttClient);

    void log(float lux);
    void maybeAlert(float lux);


private:
    std::shared_ptr<IMqttClient> mqttClient_;
    float lastLux_ = -1.0f;
};
#endif //LUX_LOGGER_HPP
