#include "SmartMqttOrchestrator.hpp"
#include <sstream>
#include <cstdlib>
#include <iostream>

SmartMqttOrchestrator::SmartMqttOrchestrator(std::shared_ptr<IMqttClient> mqttClient,
                                             std::shared_ptr<SmartRuleEngine> ruleEngine)
        : mqttClient_(std::move(mqttClient)), ruleEngine_(std::move(ruleEngine)) {}

void SmartMqttOrchestrator::start() {
    // lambda simply forwards the payload to method onMotionMessage
    mqttClient_->subscribe("/sensor/motion", [this](const std::string& payload) {
        onMotionMessage(payload);
    });
    mqttClient_->subscribe("/sensor/lux", [this](const std::string& payload) {
        onLuxMessage(payload);
    });
    mqttClient_->subscribe("/sensor/temperature", [this](const std::string& payload) {
        onTempMessage(payload);
    });
    mqttClient_->subscribe("/sensor/humidity", [this](const std::string& payload) {
        onHumidityMessage(payload);
    });
}

void SmartMqttOrchestrator::onMotionMessage(const std::string& payload) {
    std::lock_guard<std::mutex> lock(dataMutex_);
    motion_ = (payload == "true");
    hasMotion_ = true;
    evaluateIfReady();
}

void SmartMqttOrchestrator::onLuxMessage(const std::string& payload) {
    std::lock_guard<std::mutex> lock(dataMutex_);
    try {
        lux_ = std::stof(payload);
        hasLux_ = true;
        evaluateIfReady();
    } catch (const std::exception& e) {
        std::cerr << "Error converting lux payload to float: " << e.what() << std::endl;
    }
}

void SmartMqttOrchestrator::onTempMessage(const std::string& payload) {
    std::lock_guard<std::mutex> lock(dataMutex_);
    try {
        temp_ = std::stof(payload);
        hasTemp_ = true;
        evaluateIfReady();
    } catch (const std::exception& e) {
        std::cerr << "Error converting temperature payload to float: " << e.what() << std::endl;
    }
}

void SmartMqttOrchestrator::onHumidityMessage(const std::string& payload) {
    std::lock_guard<std::mutex> lock(dataMutex_);
    try {
        humidity_ = std::stof(payload);
        hasHumidity_ = true;
        evaluateIfReady();
    } catch (const std::exception& e) {
        std::cerr << "Error converting humidity payload to float: " << e.what() << std::endl;
    }
}

void SmartMqttOrchestrator::evaluateIfReady() {
    if (hasMotion_ && hasLux_ && hasTemp_ && hasHumidity_) {
        ruleEngine_->evaluate(lux_, temp_, humidity_, motion_);

        // reset flags: only want to evaluate when all 4 update again
        /*
         * What if one sensor updates much faster?
         * If one sensor floods with updates and others rarely update, evaluation may stall.
         * We can improve by also triggering evaluation after some timeout or partial data (depending on our tolerance).
         * */
        hasMotion_ = false;
        hasLux_ = false;
        hasTemp_ = false;
        hasHumidity_ = false;
    }
}
