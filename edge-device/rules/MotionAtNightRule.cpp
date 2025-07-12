#include "MotionAtNightRule.hpp"
#include <nlohmann/json.hpp>
#include "../../utils/Logging.hpp"
#include "../../utils/TimeUtils.hpp"
#include <utility>

MotionAtNightRule::MotionAtNightRule(std::shared_ptr <LightController> lightController,
                                     std::shared_ptr<IMqttClient> mqttClient,
                                     float luxThreshold)
        : lightController_(std::move(lightController)),
          mqttClient_(std::move(mqttClient)),
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

        nlohmann::json payloadJson = {
                {"device_id", mqttClient_->getClientId()},
                {"timestamp", getCurrentIsoTimestampUTC()},
                {"sensor", "motion"},
                {"value", motion},
                {"rule", "MotionAtNightRule"},
                {"message", "MotionAtNight: Light turned ON"}
        };
        std::string payload = payloadJson.dump();
        threadSafeLog("[Inside MotionAtNightRule::evaluate] Publishing alert: attempting to publish to alerts/light");
        mqttClient_->publish("alerts/light", payload);
        threadSafeLog("[MotionAtNightRule] Published alert: " + payload);
    }
    else {
        lightController_->forceOff();
        threadSafeLog("[MotionAtNightRule] Either no motion or sufficient ambient light (lux = " +
                      std::to_string(lux) + "). Turning light OFF.");
    }
}