#include "MotionMonitor.hpp"

MotionMonitor::MotionMonitor(std::shared_ptr <IMotionSensor> sensor)
    : sensor_(std::move(sensor)){}

    // TODO
MotionMonitor::MotionMonitor(std::shared_ptr <IMotionSensor> sensor, std::shared_ptr<IMqttClient> mqttClient)
    : sensor_(std::move(sensor)), mqttClient_(std::move(mqttClient)){}
void MotionMonitor::startMonitoring(int intervalMs) {
    while (true) {
        if (sensor_->isMotionDetected()) {
            std::cout << "[MotionMonitor] 🚨 Motion detected!" << std::endl;
            if (mqttClient_) {
                mqttClient_->publish("motion/detected", "true");
            }
        } else {
            std::cout << "[MotionMonitor] ...No motion" << std::endl;
            if (mqttClient_) {
                mqttClient_->publish("motion/detected", "false");
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(intervalMs));
    }
}
