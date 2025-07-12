#include "PahoMqttClient.hpp"
#include "../utils/Logging.hpp"
#include <mqtt/connect_options.h>
#include <mqtt/ssl_options.h>
#include <iostream>

PahoMqttClient::PahoMqttClient(const std::string& serverURI,
                               const std::string& clientId,
                               const std::string& certPath,
                               const std::string& keyPath,
                               const std::string& caPath)
        : client_(serverURI, clientId),
          clientId_(clientId),
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

        std::ostringstream oss;
        oss << "[DEBUG] Attempting MQTT connection to " << client_.get_server_uri() << " with client ID " << client_.get_client_id();
        threadSafeLog(oss.str());
        client_.connect(connOpts)->wait();
        threadSafeLog("[INFO] MQTT connected successfully.");

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
    static std::mutex mqttPublishMutex_;
    std::lock_guard<std::mutex> lock(mqttPublishMutex_);
    mqtt::message_ptr msg = mqtt::make_message(topic, message);
    msg->set_qos(1);
    client_.publish(msg)->wait();
    std::ostringstream oss;
    oss << "[INFO] Published message to topic: " << topic;
    threadSafeLog(oss.str());
}

void PahoMqttClient::subscribe(const std::string &topic,
                               std::function<void(const std::string &)> callback) {
    try {
        std::lock_guard<std::mutex> lock(callbacksMutex_);
        callbacks_[topic] = callback;
        client_.set_callback(*this);  // Make sure our class receives MQTT messages
        client_.subscribe(topic, 1)->wait(); // uses QoS=1 (at least once delivery) and blocks until subscription is confirmed.
        std::ostringstream oss;
        oss << "[INFO] Subscribed to topic: " << topic;
        threadSafeLog(oss.str());
    } catch (const mqtt::exception& e) {
        std::cerr << "[ERROR] MQTT exception during subscription: " << e.what() << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "[ERROR] Standard exception during subscription: " << e.what() << std::endl;
    } catch (...) {
        std::cerr << "[ERROR] Unknown error occurred during subscription." << std::endl;
    }
}

void PahoMqttClient::message_arrived(mqtt::const_message_ptr msg) {
   std::lock_guard<std::mutex> lock(callbacksMutex_);
   auto it = callbacks_.find(msg->get_topic());
   if (it != callbacks_.end()) {
       it->second(msg->to_string());
   }
}

const std::string& PahoMqttClient::getClientId() const {
    return clientId_;
}

void PahoMqttClient::disconnect() {
    client_.disconnect()->wait();
}
