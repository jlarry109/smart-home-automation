#include "MotionMonitor.hpp"
#include <iostream>
#include <thread>
#include <chrono>

MotionMonitor::MotionMonitor(std::shared_ptr <IMotionSensor> sensor)
    : sensor_(std::move(sensor)){}

    // TODO
MotionMonitor::MotionMonitor(std::shared_ptr <IMotionSensor> sensor, std::shared_ptr<IMqttClient> mqttClient)
    : sensor_(std::move(sensor)), mqttClient_(std::move(mqttClient)){}

 MotionMonitor::~MotionMonitor() {
     std::cout << "[MotionMonitor] Destructor called." << std::endl;
    stopMonitoring();
}
void MotionMonitor::startMonitoring(int intervalMs) {
    if (running_) {
        std::cout << "[MotionMonitor] Already running" << std::endl;
        return;
    }
    running_ = true;
    workerThread_ = std::thread(&MotionMonitor::monitoringLoop, this, intervalMs);
}

void MotionMonitor::stopMonitoring() {
    if (!running_) {
        std::cout << "[MotionMonitor] Already not running" << std::endl;
        return;
    }
    running_ = false;
    if (workerThread_.joinable()) {
        workerThread_.join();
    }
}

void MotionMonitor::monitoringLoop(int intervalMs) {
    try {
        while (running_) {
            bool motionDetected = sensor_->isMotionDetected();
            std::cout << "[MotionMonitor] " << (motionDetected ? "🚨 Motion detected!" : "...No motion") << std::endl;

            if (mqttClient_) {
                mqttClient_->publish("motion/detected", motionDetected ? "true" : "false");
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(intervalMs));
        }
        std::cout << "[MotionMonitor] 🛑 Monitoring stopped gracefully." << std::endl;
    } catch (const std::exception & e) {
        std::cerr << "[MotionMonitor] Error in monitoring loop: " << e.what() << std::endl;
    } catch (...) {
        std::cerr << "[MotionMonitor] Unknown error in monitoring loop." << std::endl;
    }
}
