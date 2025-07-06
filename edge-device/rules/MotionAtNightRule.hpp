#ifndef MOTION_AT_NIGHT_RULE_HPP
#define MOTION_AT_NIGHT_RULE_HPP

#include "ISmartRule.hpp"
#include "../light/LightController.hpp"  // TODO: Make sure path is correct
#include <memory>

class MotionAtNightRule : public ISmartRule {
public:
    MotionAtNightRule(std::shared_ptr<LightController> lightController, float luxThreshold = 40.0f);
    virtual ~MotionAtNightRule() = default;
    virtual void evaluate(float lux, float temp, float humidity, bool motion) override;

private:
    float luxThreshold_;
    std::shared_ptr<LightController> lightController_;
};
#endif // MOTION_AT_NIGHT_RULE_HPP
