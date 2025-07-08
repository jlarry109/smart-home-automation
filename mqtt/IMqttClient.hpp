#ifndef MQTTCLIENT_HPP
#define MQTTCLIENT_HPP

#include <string>
#include <functional>

class IMqttClient {
public:
    virtual void connect() = 0;
    virtual const std::string& getClientId() const = 0;
    virtual void publish (const std::string& topic, const std::string& message) = 0;

    // This allows any MQTT implementation to let users register a function to handle messages for a given topic.
    virtual void subscribe(const std::string& topic,
                           std::function<void(const std::string& payload)> callback) = 0;
    virtual void disconnect() = 0;
    virtual ~ IMqttClient() = default;
};
#endif // MQTTCLIENT_HPP
