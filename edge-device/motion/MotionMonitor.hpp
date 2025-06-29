#ifndef MOTION_MONITOR_HPP
#define MOTION_MONITOR_HPP

#include "IMotionSensor.hpp"
#include "IMqttClient.hpp"
#include <memory>
#include <chrono>
#include <thread>
#include <iostream>

class MotionMonitor {
public:
    explicit MotionMonitor(std::shared_ptr<IMotionSensor> sensor);

    MotionMonitor(std::shared_ptr<IMotionSensor> sensor,
                  std::shared_ptr<IMqttClient> mqttClient);

    void startMonitoring(int intervalMs = 1000);

private:
    std::shared_ptr<IMotionSensor> sensor_;
    std::shared_ptr<IMqttClient> mqttClient_;
};
#endif // MOTION_MONITOR_HPP
