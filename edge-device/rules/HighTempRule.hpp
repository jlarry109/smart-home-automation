#ifndef HIGH_TEMP_RULE_HPP
#define HIGH_TEMP_RULE_HPP

#include "ISmartRule.hpp"
#include "../../utils/Logging.hpp"
#include <memory>
#include <functional>

class HighTempRule : public ISmartRule {
public:
    HighTempRule(std::function<void()> onOverheatAction, float threshold = 30.0f);
    ~ HighTempRule() = default;

    void evaluate(float lux, float temp, float humidity, bool motion) override;

private:
    float threshold_;
    std::function<void()> onOverheatAction_;
};
#endif // HIGH_TEMP_RULE_HPP
