#ifndef ENVIRONMENT_MONITOR_HPP
#define ENVIRONMENT_MONITOR_HPP

#include "ITemperatureHumiditySensor.hpp"
#include "IMqttClient.hpp"
#include <memory>
#include <atomic>
#include <thread>

class EnvironmentMonitor {
public:
    EnvironmentMonitor(std::shared_ptr<ITemperatureHumiditySensor> sensor,
                       std::shared_ptr<IMqttClient> mqttClient);
    ~EnvironmentMonitor();

    // Starts periodic monitoring of the environment at intervalMs milliseconds.
    void startMonitoring(int intervalMs = 1000);

    // Stops monitoring gracefully
    void stopMonitoring();

private:
    void monitoringLoop(int intervalMs);

    std::shared_ptr<ITemperatureHumiditySensor> sensor_;
    std::shared_ptr<IMqttClient> mqttClient_;

    std::atomic<bool> running_{false};
    std::thread workerThread_;
};
#endif // ENVIRONMENT_MONITOR_HPP
