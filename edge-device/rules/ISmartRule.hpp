#ifndef I_SMART_RULE_HPP
#define I_SMART_RULE_HPP


class ISmartRule {
public:
    virtual ~ISmartRule() = default;
    virtual void evaluate(float lux, float temp, float humidity, bool motion) = 0;
};
#endif // I_SMART_RULE_HPP
