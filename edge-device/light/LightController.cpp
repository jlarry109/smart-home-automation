#include "LightController.hpp"
#include "../../utils/Logging.hpp"

// Lux threshold for turning on the lights
constexpr float LUX_ON_THRESHOLD = 100.0f;
constexpr float LUX_OFF_THRESHOLD = 300.0f;

// Applies lighting rules: if it’s dark, turn light on; if it’s bright, turn off.
void LightController::update(float lux) {
    if (lux < LUX_ON_THRESHOLD && !isLightOn_) {
        turnOn();
    } else if (lux > LUX_OFF_THRESHOLD && isLightOn_) {
        turnOff();
    } else {
        // No state change
    }
}

void LightController::forceOn() {
    if (!isLightOn_) {
        turnOn();
    }
}

void LightController::forceOff() {
    if (isLightOn_) {
        turnOff();
    }
}

void LightController::turnOn() {
    std::ostringstream oss;
    oss << "[LightController] 💡 Turning light ON (lux too low)";
    threadSafeLog(oss.str());
    isLightOn_ = true;

    // TODO: GPIO/relay logic will go here later
}

void LightController::turnOff() {
    std::ostringstream oss;
    oss << "[LightController] 🔌 Turning light OFF (lux sufficient)";
    isLightOn_ = false;

    // TODO: GPIO/relay logic will go here
}
