#include "SmartMqttOrchestrator.hpp"
#include <iostream>

SmartMqttOrchestrator::SmartMqttOrchestrator(std::shared_ptr<IMqttClient> mqttClient,
                                             std::shared_ptr<SmartRuleEngine> ruleEngine)
        : mqttClient_(std::move(mqttClient)), ruleEngine_(std::move(ruleEngine)) {}

void SmartMqttOrchestrator::start() {
    running_ = true;
    mqttClient_->subscribe("sensors/motion", [this](const std::string& payload) {
        std::lock_guard<std::mutex> lock(queueMutex_);
        messageQueue_.push({Message::Type::Motion, payload});
        queueCv_.notify_one();
    });
    mqttClient_->subscribe("sensors/lux", [this](const std::string& payload) {
        std::lock_guard<std::mutex> lock(queueMutex_);
        messageQueue_.push({Message::Type::Lux, payload});
        queueCv_.notify_one();
    });
    mqttClient_->subscribe("sensors/temperature", [this](const std::string& payload) {
        std::lock_guard<std::mutex> lock(queueMutex_);
        messageQueue_.push({Message::Type::Temp, payload});
        queueCv_.notify_one();
    });
    mqttClient_->subscribe("sensors/humidity", [this](const std::string& payload) {
        std::lock_guard<std::mutex> lock(queueMutex_);
        messageQueue_.push({Message::Type::Humidity, payload});
        queueCv_.notify_one();
    });

    workerThread_ = std::thread(&SmartMqttOrchestrator::processingLoop, this);
}

void SmartMqttOrchestrator::stop() {
    running_ = false;
    queueCv_.notify_all();
    if (workerThread_.joinable()) {
        workerThread_.join();
    }
}

void SmartMqttOrchestrator::processingLoop() {
    while (running_) {
        std::unique_lock<std::mutex> lock(queueMutex_);
        queueCv_.wait(lock, [this] {
            return !messageQueue_.empty() || !running_;
        });

        while (!messageQueue_.empty()) {
            Message msg = messageQueue_.front();
            messageQueue_.pop();
            lock.unlock();

            switch (msg.type) {
                case Message::Type::Motion:
                    onMotionMessage(msg.payload);
                    break;
                case Message::Type::Lux:
                    onLuxMessage(msg.payload);
                    break;
                case Message::Type::Temp:
                    onTempMessage(msg.payload);
                    break;
                case Message::Type::Humidity:
                    onHumidityMessage(msg.payload);
                    break;
            }

            lock.lock();
        }
    }
}


void SmartMqttOrchestrator::onMotionMessage(const std::string& payload) {
    bool val  = (payload == "true");
    {
        std::lock_guard<std::mutex> lock(dataMutex_);
        motion_ = val;
        hasMotion_ = true;
    }
    evaluateIfReady();
}

void SmartMqttOrchestrator::onLuxMessage(const std::string& payload) {
    try {
        float val = std::stof(payload);
        {
            std::lock_guard<std::mutex> lock(dataMutex_);
            lux_ = val;
            hasLux_ = true;
        }
        evaluateIfReady();
    } catch (const std::exception& e) {
        std::cerr << "Error converting lux payload to float: " << e.what() << std::endl;
    }
}

void SmartMqttOrchestrator::onTempMessage(const std::string& payload) {
    try {
        float val = std::stof(payload);
        {
            std::lock_guard<std::mutex> lock(dataMutex_);
            temp_ = val;
            hasTemp_ = true;
        }
        evaluateIfReady();
    } catch (const std::exception& e) {
        std::cerr << "Error converting temperature payload to float: " << e.what() << std::endl;
    }
}

void SmartMqttOrchestrator::onHumidityMessage(const std::string& payload) {
    try {
        float val = std::stof(payload);
        {
            std::lock_guard<std::mutex> lock(dataMutex_);
            humidity_ = val;
            hasHumidity_ = true;
        }
        evaluateIfReady();
    } catch (const std::exception& e) {
        std::cerr << "Error converting humidity payload to float: " << e.what() << std::endl;
    }
}

void SmartMqttOrchestrator::evaluateIfReady() {
    bool ready = false;
    float lux, temp, humidity;
    bool motion;

    {
        std::lock_guard<std::mutex> lock(dataMutex_);
        if (hasMotion_ && hasLux_ && hasTemp_ && hasHumidity_) {
            lux = lux_;
            temp = temp_;
            humidity = humidity_;
            motion = motion_;
            hasMotion_ = hasLux_ = hasTemp_ = hasHumidity_ = false;
            ready = true;
        }
    }

    if (ready) {
        std::ostringstream oss;
        oss << "[DEBUG] Evaluating rules with values: "
            << "Lux=" << lux << ", Temp=" << temp
            << ", Humidity=" << humidity << ", Motion=" << motion;
        threadSafeLog(oss.str());

        ruleEngine_->evaluate(lux, temp, humidity, motion);
    }
}


