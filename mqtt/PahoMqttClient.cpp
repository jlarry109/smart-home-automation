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
          caPath_(caPath) {
    // Start background publisher
    publishWorker_ = std::thread(&PahoMqttClient::publishWorkerLoop, this);
}

PahoMqttClient::~PahoMqttClient() {
    {
        std::lock_guard<std::mutex> lock(publishQueueMutex_);
        stopWorker_ = true;
    }
    publishCv_.notify_all();
    if (publishWorker_.joinable()) {
        publishWorker_.join();
    }
}

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
    threadSafeLog("[Inside PahoMqttClient::publish]...looking to publish to: " + topic);

    // Build message without holding the mutex
    mqtt::message_ptr msg = mqtt::make_message(topic, message);
    msg->set_qos(1);
    threadSafeLog("[Inside PahoMqttClient::publish - after set_qos()]...looking to publish to: " + topic);

    // Acquire token under lock, then release the lock before waiting on it.
    mqtt::delivery_token_ptr tok;
    try {
        {
            std::lock_guard<std::mutex> lock(mqttPublishMutex_);
            tok = client_.publish(msg);
        }

        if (!tok) {
            std::ostringstream oss;
            oss << "[ERROR] Publish returned null token for topic: " << topic;
            threadSafeLog(oss.str());
            return;
        }

        // Wait for completion outside the mutex to avoid serializing all publishers.
        constexpr auto WAIT_TIMEOUT = std::chrono::seconds(3);
        if (tok->wait_for(WAIT_TIMEOUT)) {
            std::ostringstream oss;
            oss << "[INFO] Published message to topic: " << topic;
            threadSafeLog(oss.str());
        } else {
            std::ostringstream oss;
            oss << "[WARN] Publish timeout for topic: " << topic;
            threadSafeLog(oss.str());
        }

    } catch (const mqtt::exception& e) {
        std::ostringstream oss;
        oss << "[MQTT EXCEPTION] Failed to publish to topic " << topic << ": " << e.what();
        threadSafeLog(oss.str());
    } catch (const std::exception& e) {
        std::ostringstream oss;
        oss << "[EXCEPTION] Failed to publish to topic " << topic << ": " << e.what();
        threadSafeLog(oss.str());
    } catch (...) {
        threadSafeLog("[UNKNOWN EXCEPTION] Failed to publish (unknown) for topic: " + topic);
    }
}

void PahoMqttClient::publishAsync(const std::string& topic, const std::string& message, int qos, int retries) {
    if (retries < 0) {
        std::lock_guard<std::mutex> lock(retryPolicyMutex_);
        retries = retryPolicy_.maxRetries;
    }
    {
        std::lock_guard<std::mutex> lock(publishQueueMutex_);
        publishQueue_.push(PendingPublish{topic, message, qos, retries});
    }
    publishCv_.notify_one();
}

void PahoMqttClient::publishWorkerLoop() {
    while (true) {
        PendingPublish job;
        {
            std::unique_lock<std::mutex> lock(publishQueueMutex_);
            publishCv_.wait(lock, [&] { return stopWorker_ || !publishQueue_.empty(); });
            if (stopWorker_ && publishQueue_.empty()) break;
            job = std::move(publishQueue_.front());
            publishQueue_.pop();
        }

        RetryPolicy policyCopy;
        {
            std::lock_guard<std::mutex> lock(retryPolicyMutex_);
            policyCopy = retryPolicy_;
        }

        int attempt = 0;
        auto backoff = policyCopy.initialBackoff;
        while (attempt < job.retries) {
            try {
                mqtt::message_ptr msg = mqtt::make_message(job.topic, job.payload);
                msg->set_qos(job.qos);

                mqtt::delivery_token_ptr tok;
                {
                    std::lock_guard<std::mutex> lock(mqttPublishMutex_);
                    tok = client_.publish(msg);
                }

                logTokenId(tok, "[ASYNC] Publish initiated");

                if (tok && tok->wait_for(std::chrono::seconds(3))) {
                    logTokenId(tok, "[ASYNC] Publish successful");
                    break;
                } else {
                    logTokenId(tok, "[ASYNC] Publish timeout");
                }
            } catch (const mqtt::exception& e) {
                threadSafeLog(std::string("[ASYNC] Publish failed: ") + e.what());
            }

            ++attempt;
            if (attempt < job.retries) {
                std::this_thread::sleep_for(backoff);
                backoff = std::chrono::milliseconds(
                        static_cast<int64_t>(backoff.count() * policyCopy.backoffMultiplier)
                );
            }
        }
    }
}

void PahoMqttClient::setRetryPolicy(const RetryPolicy& policy) {
    std::lock_guard<std::mutex> lock(retryPolicyMutex_);
    retryPolicy_ = policy;
}

void PahoMqttClient::logTokenId(const mqtt::delivery_token_ptr& tok, const std::string& prefix) {
    std::ostringstream oss;
    oss << prefix;
    if (tok) {
        try {
            oss << " (token id=" << tok->get_message_id() << ")";
        } catch (...) {
            oss << " (token id unavailable)";
        }
    }
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


/*
 * // Set retry/backoff policy at runtime
RetryPolicy policy;
policy.maxRetries = 5;
policy.initialBackoff = std::chrono::milliseconds(300);
policy.backoffMultiplier = 1.5;
mqttClient->setRetryPolicy(policy);

// Fire-and-forget async publish
mqttClient->publishAsync("alerts/temperature", "Temp high!", 1);
 */