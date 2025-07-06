#ifndef SMART_ORCHESTRATOR_HPP
#define SMART_ORCHESTRATOR_HPP

#include "../rules/SmartRuleEngine.hpp"
#include "../light/ILightSensor.hpp"
#include "../motion/IMotionSensor.hpp"
#include "../temperature_humidity/ITemperatureHumiditySensor.hpp"
#include <memory>

/* centralize sensor polling and rule evaluation */

class SmartOrchestrator {
public:
    SmartOrchestrator(std::shared_ptr<ILightSensor> lightSensor,
                       std::shared_ptr<IMotionSensor> motionSensor,
                       std::shared_ptr<ITemperatureHumiditySensor> tempHumiditySensor,
                       std::shared_ptr<SmartRuleEngine> ruleEngine);

    void run(int intervalMs = 10000);

private:
    std::shared_ptr<ILightSensor> lightSensor_;
    std::shared_ptr<IMotionSensor> motionSensor_;
    std::shared_ptr<ITemperatureHumiditySensor> tempHumiditySensor_;
    std::shared_ptr<SmartRuleEngine> ruleEngine_;
};
#endif // SMART_ORCHESTRATOR_HPP

