#ifndef MOTION_MONITOR_HPP
#define MOTION_MONITOR_HPP

#include "IMotionSensor.hpp"
#include <memory>
#include <chrono>
#include <thread>
#include <iostream>

class MotionMonitor {
public:
    explicit MotionMonitor(std::shared_ptr<IMotionSensor> sensor);

    void startMonitoring(int intervalMs = 1000);

private:
    std::shared_ptr<IMotionSensor> sensor_;
};
#endif
