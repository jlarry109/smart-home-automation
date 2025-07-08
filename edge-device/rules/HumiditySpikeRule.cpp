#include "HumiditySpikeRule.hpp"
#include "../../utils/Logging.hpp"

HumiditySpikeRule::HumiditySpikeRule(std::function<void(float)> onHighHumidityAction, float threshold)
    : onHighHumidityAction_(std::move(onHighHumidityAction)), threshold_(threshold) {}


void HumiditySpikeRule::evaluate(float lux, float temp, float humidity, bool motion) {
    if (humidity > threshold_) {
        threadSafeLog("[HumiditySpikeRule]: High humidity detected: " + std::to_string(humidity) + "%");
        onHighHumidityAction_(humidity); // Trigger alert (e.g., publish to MQTT)
    }
}

HumiditySpikeRule::~HumiditySpikeRule() = default;
