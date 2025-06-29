#ifndef MQTTCLIENT_HPP
#define MQTTCLIENT_HPP

#include <string>

class IMqttClient {
public:
    virtual void connect() = 0;
    virtual void publish (const std::string& topic, const std::string& message) = 0;
    virtual void disconnect() = 0;
    virtual ~ IMqttClient() = default;
};
#endif // MQTTCLIENT_HPP
