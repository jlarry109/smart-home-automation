#ifndef LIGHT_MONITOR_HPP
#define LIGHT_MONITOR_HPP

#include "ILightSensor.hpp"
#include "LuxLogger.hpp"
#include "LightController.hpp"
#include <memory>
#include <thread>
#include <atomic>
/*
 * Orchestrates all 3 above
 * Periodically reads from sensor, updates controller, logs to MQTT
 * */

class LightMonitor {
public:
    LightMonitor(std::shared_ptr<ILightSensor> sensor,
                 std::shared_ptr<LuxLogger> logger,
                 std::shared_ptr<LightController> controller);
    ~LightMonitor();

    void startMonitoring(int intervalMs = 1000); // default: every 1 seconds
    void stopMonitoring();

private:
    void monitoringLoop(int intervalMs);


    std::shared_ptr<ILightSensor> sensor_;
    std::shared_ptr<LuxLogger> logger_;
    std::shared_ptr<LightController> controller_;

    std::atomic<bool> running_{false};
    std::thread monitorThread_;
};
#endif //LIGHT_MONITOR_HPP
