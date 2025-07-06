#include "SmartRuleEngine.hpp"


void SmartRuleEngine::addRule(std::shared_ptr<ISmartRule> rule) {
    rules.push_back(rule);
}

void SmartRuleEngine::evaluate(float lux, float temp, float humidity, bool motion) {
    threadSafeLog("[SmartRuleEngine] Evaluating rules...");
    for (auto& rule : rules) {
        rule->evaluate(lux, temp, humidity, motion);
    }
}