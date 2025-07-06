#include "HumiditySpikeRule.hpp"
#include "../../utils/Logging.hpp"

HumiditySpikeRule::HumiditySpikeRule(std::function<void()> onHighHumidityAction, float threshold)
    : onHighHumidityAction_(std::move(onHighHumidityAction)), threshold_(threshold) {}


void HumiditySpikeRule::evaluate(float lux, float temp, float humidity, bool motion) {
    if (humidity > threshold_) {
        threadSafeLog("[HumiditySpikeRule]: High humidity detected: " + std::to_string(humidity) + "%");
        onHighHumidityAction_(); // Trigger alert (e.g., publish to MQTT)
    }
}

/*
 * auto mqttClient = std::make_shared<YourMqttClientImpl>();
auto humidityRule = std::make_shared<HumiditySpikeRule>([mqttClient] {
    mqttClient->publish("alerts/humidity", "💧 Humidity spike detected!");
});
 * */