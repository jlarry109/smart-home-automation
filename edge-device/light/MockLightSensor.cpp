#include "MockLightSensor.hpp"

#include <chrono>


MockLightSensor::MockLightSensor()
    : engine_(std::chrono::system_clock::now().time_since_epoch().count()),
      dist_(0.0f, 800.0f) {} // Simulates lux values from 0 to 800


float MockLightSensor::readLux() const {
    return dist_(engine_);
}