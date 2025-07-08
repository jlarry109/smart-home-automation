#include "HighTempRule.hpp"


HighTempRule::HighTempRule(std::function<void(float)> onOverheatAction, float threshold)
    : onOverheatAction_(std::move(onOverheatAction)), threshold_(threshold) {}

void HighTempRule::evaluate(float lux, float temp, float humidity, bool motion) {
    if (temp > threshold_) {
        threadSafeLog("[HighTempRule]: High temperature detected: " + std::to_string(temp) + "°C");
        onOverheatAction_(temp); // Trigger MQTT alert or buzzer action...
    }
}


/*
 *  TODO: Implement the HighTempRule class methods here.
 *  The rule should trigger an action when the temperature exceeds the threshold.
 *  The action is defined by the onOverheatAction_ function passed to the constructor.
 *  auto mqttClient = std::make_shared<YourMqttClientImpl>();
auto highTempRule = std::make_shared<HighTempRule>([mqttClient] {
    mqttClient->publish("alerts/temperature", "🔥 Temperature exceeds safe threshold!");
});
 */

