#ifndef PAHO_MQTT_HPP
#define PAHO_MQTT_HPP

#include "IMqttClient.hpp"
#include <mqtt/async_client.h>
#include <map>
#include <mutex>

class PahoMqttClient : public IMqttClient, public virtual mqtt::callback {

public:
    PahoMqttClient(const std::string& serverURI,
                   const std::string& clientId,
                   const std::string& certPath,
                   const std::string& keyPath,
                   const std::string& caPath);

    void connect() override;
    void publish(const std::string& topic, const std::string& message) override;
    void subscribe(const std::string& topic,
                   std::function<void(const std::string& payload)> callback) override;
    void message_arrived(mqtt::const_message_ptr msg) override;
    void disconnect() override;

private:
    mqtt::async_client client_;
    std::string certPath_;
    std::string keyPath_;
    std::string caPath_;

    std::map<std::string, std::function<void(const std::string& payload)>> callbacks_;
    std::mutex callbacksMutex_;
};
#endif // PAHO_MQTT_HPP


