#include "SmartOrchestrator.hpp"
#include <thread>

SmartOrchestrator::SmartOrchestrator(std::shared_ptr<ILightSensor> lightSensor,
                                     std::shared_ptr<IMotionSensor> motionSensor,
                                     std::shared_ptr<ITemperatureHumiditySensor> tempHumiditySensor,
                                     std::shared_ptr<IMqttClient> mqttClient,
                                     std::shared_ptr<SmartRuleEngine> ruleEngine)
        : lightSensor_(std::move(lightSensor)),
          motionSensor_(std::move(motionSensor)),
          tempHumiditySensor_(std::move(tempHumiditySensor)),
          mqttClient_(std::move(mqttClient)),
          ruleEngine_(std::move(ruleEngine)) {}

void SmartOrchestrator::startAll() {
    // Logger + controller
    auto lightLogger = std::make_shared<LuxLogger>(mqttClient_);
    auto lightController = std::make_shared<LightController>();

    // Monitors
    motionMonitor_ = std::make_shared<MotionMonitor>(motionSensor_, mqttClient_);
    envMonitor_ = std::make_shared<EnvironmentMonitor>(tempHumiditySensor_, mqttClient_);
    lightMonitor_ = std::make_shared<LightMonitor>(lightSensor_, lightLogger, lightController);

    motionMonitor_->startMonitoring(5000);
    envMonitor_->startMonitoring(7000);
    lightMonitor_->startMonitoring(10000);
}

void SmartOrchestrator::stopAll() {
    if (motionMonitor_) motionMonitor_->stopMonitoring();
    if (envMonitor_) envMonitor_->stopMonitoring();
    if (lightMonitor_) lightMonitor_->stopMonitoring();
}
