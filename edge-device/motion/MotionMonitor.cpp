#include "MotionMonitor.hpp"
#include "../../utils/Logging.hpp"
#include <iostream>
#include <thread>
#include <chrono>

MotionMonitor::MotionMonitor(std::shared_ptr<IMotionSensor> sensor)
        : sensor_(std::move(sensor)) {}

MotionMonitor::MotionMonitor(std::shared_ptr<IMotionSensor> sensor, std::shared_ptr<IMqttClient> mqttClient)
        : sensor_(std::move(sensor)), mqttClient_(std::move(mqttClient)) {}

MotionMonitor::~MotionMonitor() {
    threadSafeLog("[MotionMonitor] Destructor called.");
    stopMonitoring();
}

void MotionMonitor::startMonitoring(int intervalMs) {
    if (running_) {
        threadSafeLog("[MotionMonitor] Already running");
        return;
    }
    running_ = true;
    workerThread_ = std::thread(&MotionMonitor::monitoringLoop, this, intervalMs);
}

void MotionMonitor::stopMonitoring() {
    if (!running_) {
        threadSafeLog("[MotionMonitor] Already not running");
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
            std::ostringstream oss;
            oss << "[MotionMonitor] " << (motionDetected ? " Motion detected!" : "...No motion");
            threadSafeLog(oss.str());

            if (mqttClient_) {
                mqttClient_->publish("motion/detected", motionDetected ? "true" : "false");
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(intervalMs));
        }
        threadSafeLog("[MotionMonitor] Monitoring stopped gracefully.");
    } catch (const std::exception& e) {
        std::cerr << "[MotionMonitor] Error in monitoring loop: " << e.what() << std::endl;
    } catch (...) {
        std::cerr << "[MotionMonitor] Unknown error in monitoring loop." << std::endl;
    }
}
