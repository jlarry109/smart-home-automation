#include "PahoMqttClient.hpp"
#include <mqtt/connect_options.h>
#include <mqtt/ssl_options.h>
#include <iostream>

PahoMqttClient::PahoMqttClient(const std::string& serverURI,
                               const std::string& clientId,
                               const std::string& certPath,
                               const std::string& keyPath,
                               const std::string& caPath)
        : client_(serverURI, clientId),
          certPath_(certPath),
          keyPath_(keyPath),
          caPath_(caPath) {}

void PahoMqttClient::connect() {
    mqtt::ssl_options sslOpts;
    sslOpts.set_key_store(certPath_);   // Device certificate
    sslOpts.set_private_key(keyPath_);  // Private key
    sslOpts.set_trust_store(caPath_);   // Root CA

    mqtt::connect_options connOpts;
    connOpts.set_clean_session(true);
    connOpts.set_ssl(sslOpts);

    client_.connect(connOpts)->wait();
}

void PahoMqttClient::publish(const std::string& topic, const std::string& message) {
    mqtt::message_ptr msg = mqtt::make_message(topic, message);
    msg->set_qos(1);
    client_.publish(msg)->wait();
}

void PahoMqttClient::disconnect() {
    client_.disconnect()->wait();
}
