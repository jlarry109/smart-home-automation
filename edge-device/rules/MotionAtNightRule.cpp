#include "MotionAtNightRule.hpp"
#include "../../utils/Logging.hpp"
#include <utility>

MotionAtNightRule::MotionAtNightRule(std::shared_ptr <LightController> lightController, float luxThreshold)
        : lightController_(std::move(lightController)),
        luxThreshold_(luxThreshold)
        {}

void MotionAtNightRule::evaluate(float lux, float temp, float humidity, bool motion) {
    // In this rule, we assume:
    // - "Night" means low ambient light (lux < luxThreshold_)
    // -  Motion at night should turn the light on
    // - Otherwise, turn the light off

    if (motion && lux < luxThreshold_) {
        lightController_->forceOn();
        threadSafeLog("[MotionAtNightRule] Motion detected at low light (lux = " +
                      std::to_string(lux) + "). Turning light ON.");
    }
    else {
        lightController_->forceOff();
        threadSafeLog("[MotionAtNightRule] Either no motion or sufficient ambient light (lux = " +
                      std::to_string(lux) + "). Turning light OFF.");
    }
}