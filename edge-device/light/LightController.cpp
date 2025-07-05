#include "LightController.hpp"

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

void LightController::turnOn() {
    std::cout << "[LightController] 💡 Turning light ON (lux too low)" << std::endl;
    isLightOn_ = true;

    // TODO: GPIO/relay logic will go here later
}

void LightController::turnOff() {
    std::cout << "[LightController] 🔌 Turning light OFF (lux sufficient)" << std::endl;
    isLightOn_ = false;

    // TODO: GPIO/relay logic will go here
}
