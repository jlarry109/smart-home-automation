#include "MotionMonitor.hpp"

MotionMonitor::MotionMonitor(std::shared_ptr <IMotionSensor> sensor)
    : sensor_(std::move(sensor)){}

void MotionMonitor::startMonitoring(int intervalMs) {
    while (true) {
        if (sensor_->isMotionDetected()) {
            std::cout << "[MotionMonitor] 🚨 Motion detected!" << std::endl;
        } else {
            std::cout << "[MotionMonitor] ...No motion" << std::endl;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(intervalMs));
    }
}
