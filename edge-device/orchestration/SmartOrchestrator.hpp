#ifndef SMART_ORCHESTRATOR_HPP
#define SMART_ORCHESTRATOR_HPP

#include "../rules/SmartRuleEngine.hpp"
#include "../mqtt/IMqttClient.hpp"
#include "../light/ILightSensor.hpp"
#include "../motion/IMotionSensor.hpp"
#include "../temperature_humidity/ITemperatureHumiditySensor.hpp"
#include "../motion/MotionMonitor.hpp"
#include "../temperature_humidity/EnvironmentMonitor.hpp"
#include "../light/LuxLogger.hpp"
#include "../light/LightController.hpp"
#include "../light/LightMonitor.hpp"

#include <memory>

class SmartOrchestrator {
public:
    SmartOrchestrator(std::shared_ptr<ILightSensor> lightSensor,
                      std::shared_ptr<IMotionSensor> motionSensor,
                      std::shared_ptr<ITemperatureHumiditySensor> tempHumiditySensor,
                      std::shared_ptr<IMqttClient> mqttClient,
                      std::shared_ptr<SmartRuleEngine> ruleEngine);

    void startAll();
    void stopAll();

private:
    std::shared_ptr<ILightSensor> lightSensor_;
    std::shared_ptr<IMotionSensor> motionSensor_;
    std::shared_ptr<ITemperatureHumiditySensor> tempHumiditySensor_;
    std::shared_ptr<IMqttClient> mqttClient_;
    std::shared_ptr<SmartRuleEngine> ruleEngine_;

    std::shared_ptr<MotionMonitor> motionMonitor_;
    std::shared_ptr<EnvironmentMonitor> envMonitor_;
    std::shared_ptr<LightMonitor> lightMonitor_;
};
#endif // SMART_ORCHESTRATOR_HPP
