#ifndef SMART_RULE_ENGINE_HPP
#define SMART_RULE_ENGINE_HPP
#include "ISmartRule.hpp"
#include "../../utils/Logging.hpp"
#include <vector>

class SmartRuleEngine {
public:
    void addRule(std::shared_ptr<ISmartRule> rule);
    void evaluate(float lux, float temp, float humidity, bool motion);

private:
    std::vector<std::shared_ptr<ISmartRule>> rules;
};
#endif // SMART_RULE_ENGINE_HPP
