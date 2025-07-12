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
            // Copy values
            lux = lux_;
            temp = temp_;
            humidity = humidity_;
            motion = motion_;
            ready = true;

            // Optional: reset flags if needed
            // hasMotion_ = false;
            // hasLux_ = false;
            // hasTemp_ = false;
            // hasHumidity_ = false;
        }
    }
    if (ready) {
        ruleEngine_->evaluate(lux, temp, humidity, motion);
    }
}

