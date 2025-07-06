#ifndef HUMIDITY_SPIKE_RULE_HPP
#define HUMIDITY_SPIKE_RULE_HPP

#include "ISmartRule.hpp"
#include <functional>

class HumiditySpikeRule : public ISmartRule {
public:
    HumiditySpikeRule(std::function<void()> onHighHumidityAction, float threshold = 70.0f);
    virtual ~HumiditySpikeRule();

    void evaluate(float lux, float temp, float humidity, bool motion) override;


private:
    float threshold_;
    std::function<void()> onHighHumidityAction_;
};
#endif // HUMIDITY_SPIKE_RULE_HPP
