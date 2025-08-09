#ifndef PAHO_MQTT_HPP
#define PAHO_MQTT_HPP

#include "IMqttClient.hpp"
#include <mqtt/async_client.h>
#include <map>
#include <queue>
#include <mutex>
#include <condition_variable>

struct PendingPublish {
    std::string topic;
    std::string payload;
    int qos;
    int retries;
};

struct RetryPolicy {
    int maxRetries = 3;
    std::chrono::milliseconds initialBackoff{500};
    double backoffMultiplier = 2.0; // exponential
};
class PahoMqttClient : public IMqttClient, public virtual mqtt::callback {

public:
    PahoMqttClient(const std::string& serverURI,
                   const std::string& clientId,
                   const std::string& certPath,
                   const std::string& keyPath,
                   const std::string& caPath);

    ~PahoMqttClient();
    void setRetryPolicy(const RetryPolicy& policy);
    void connect() override;
    const std::string& getClientId() const override;
    void publish(const std::string& topic, const std::string& message) override;
    void publishAsync(const std::string& topic, const std::string& message, int qos = 1, int retries = 3) override;
    void subscribe(const std::string& topic,
                   std::function<void(const std::string& payload)> callback) override;
    void message_arrived(mqtt::const_message_ptr msg) override;
    void disconnect() override;

private:
    mqtt::async_client client_;
    std::string clientId_;
    std::string certPath_;
    std::string keyPath_;
    std::string caPath_;

    std::map<std::string, std::function<void(const std::string& payload)>> callbacks_;
    std::mutex callbacksMutex_;

    // Protects Paho publish calls
    mutable std::mutex mqttPublishMutex_;

    // Queue + worker thread for handling async publishes
    std::queue<PendingPublish> publishQueue_;
    std::mutex publishQueueMutex_;
    std::condition_variable publishCv_;
    std::thread publishWorker_;
    bool stopWorker_ = false;

    // Retry config
    RetryPolicy retryPolicy_;
    std::mutex retryPolicyMutex_;

    void publishWorkerLoop();

    // Helper for token logging
    void logTokenId(const mqtt::delivery_token_ptr& tok, const std::string& prefix);
};
#endif // PAHO_MQTT_HPP


