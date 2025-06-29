#ifndef PAHO_MQTT_HPP
#define PAHO_MQTT_HPP

#include "IMqttClient.hpp"
#include <mqtt/async_client.h>
#include <string>

class PahoMqttClient : public IMqttClient {
public:
    PahoMqttClient(const std::string& serverURI,
                   const std::string& clientId,
                   const std::string& certPath,
                   const std::string& keyPath,
                   const std::string& caPath);

    void connect() override;
    void publish(const std::string& topic, const std::string& message) override;
    void disconnect() override;

private:
    mqtt::async_client client_;
    std::string certPath_;
    std::string keyPath_;
    std::string caPath_;
};
#endif // PAHO_MQTT_HPP


