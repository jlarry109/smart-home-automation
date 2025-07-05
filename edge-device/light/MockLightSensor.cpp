#include "MockLightSensor.hpp"

#include <chrono>


MockLightSensor::MockLightSensor()
    : engine_(std::chrono::system_clock::now().time_since_epoch().count()),
      dist_(0.0f, 800.0f), // Simulates lux values from 0 to 800
      callCount_(0) {}


float MockLightSensor::readLux() const {
    callCount_++;

    if (callCount_ % 15 == 0) {
        // night/dusk
        std::uniform_real_distribution<float> nightDist(0.0f, 30.0f);
        return nightDist(engine_);
    }
    // Otherwise return normal ambient variation
    return dist_(engine_);
}