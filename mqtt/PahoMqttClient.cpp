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
    try {
        mqtt::ssl_options sslOpts;
        sslOpts.set_key_store(certPath_);   // Device certificate
        sslOpts.set_private_key(keyPath_);  // Private key
        sslOpts.set_trust_store(caPath_);   // Root CA

        mqtt::connect_options connOpts;
        connOpts.set_clean_session(true);
        connOpts.set_ssl(sslOpts);

        std::cout << "[DEBUG] Attempting MQTT connection to " << client_.get_server_uri() << " with client ID " << client_.get_client_id() << std::endl;
        client_.connect(connOpts)->wait();
        std::cout << "[INFO] MQTT connected successfully." << std::endl;

    } catch (const mqtt::exception& e) {
        std::cerr << "[ERROR] MQTT exception: " << e.what() << " (code " << e.get_reason_code() << ")" << std::endl;
        throw;
    } catch (const std::exception& e) {
        std::cerr << "[ERROR] Standard exception during MQTT connect: " << e.what() << std::endl;
        throw;
    } catch (...) {
        std::cerr << "[ERROR] Unknown error occurred during MQTT connect." << std::endl;
        throw;
    }
}

void PahoMqttClient::publish(const std::string& topic, const std::string& message) {
    mqtt::message_ptr msg = mqtt::make_message(topic, message);
    msg->set_qos(1);
    client_.publish(msg)->wait();
}

void PahoMqttClient::disconnect() {
    client_.disconnect()->wait();
}
