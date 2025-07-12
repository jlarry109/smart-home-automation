#ifndef SMART_MQTT_ORCHESTRATOR_HPP
#define SMART_MQTT_ORCHESTRATOR_HPP

#include "../rules/SmartRuleEngine.hpp"
#include "../mqtt/IMqttClient.hpp"

#include <memory>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <thread>
#include <queue>

class SmartMqttOrchestrator {
public:
    SmartMqttOrchestrator(std::shared_ptr<IMqttClient> mqttClient,
                          std::shared_ptr<SmartRuleEngine> ruleEngine);

    void start();
    void stop();

private:
    void onMotionMessage(const std::string& payload);
    void onLuxMessage(const std::string& payload);
    void onTempMessage(const std::string& payload);
    void onHumidityMessage(const std::string& payload);
    void evaluateIfReady();
    void processingLoop();

    struct Message {
        enum class Type { Motion, Lux, Temp, Humidity } type;
        std::string payload;
    };

    std::shared_ptr<IMqttClient> mqttClient_;
    std::shared_ptr<SmartRuleEngine> ruleEngine_;
    std::queue<Message> messageQueue_;
    std::mutex queueMutex_;
    std::condition_variable queueCv_;
    std::atomic<bool> running_ = false;
    std::thread workerThread_;

    void processMessages();

    std::mutex dataMutex_;
    bool hasMotion_ = false;
    bool hasLux_ = false;
    bool hasTemp_ = false;
    bool hasHumidity_ = false;

    bool motion_ = false;
    float lux_ = 0.0f;
    float temp_ = 0.0f;
    float humidity_ = 0.0f;
};

#endif // SMART_MQTT_ORCHESTRATOR_HPP