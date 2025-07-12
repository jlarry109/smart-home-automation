#include "LightController.hpp"
#include "../../utils/Logging.hpp"

// Lux threshold for turning on the lights
constexpr float LUX_ON_THRESHOLD = 100.0f;
constexpr float LUX_OFF_THRESHOLD = 300.0f;


void LightController::forceOn() {
    if (!isLightOn_) {
        turnOn();
        threadSafeLog("[LightController] Light forcefully turned ON by rule");
    }
}

void LightController::forceOff() {
    if (isLightOn_) {
        turnOff();
        threadSafeLog("[LightController] Light forcefully turned OFF by rule");
    }
}

void LightController::turnOn() {
    threadSafeLog("[LightController] 💡 Turning light ON (lux too low)");
    isLightOn_ = true;
    // TODO: GPIO/relay logic will go here later
}

void LightController::turnOff() {
    std::ostringstream oss;
    threadSafeLog("[LightController] 🔌 Turning light OFF (lux sufficient)");
    isLightOn_ = false;

    // TODO: GPIO/relay logic will go here
}
