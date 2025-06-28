#include "MockMotionSensor.hpp"

MockMotionSensor::MockMotionSensor()
    : engine(std::chrono::system_clock::now().time_since_epoch().count()),
      distribution(0.3) // Bernoulli distribution that returns true ~30% of the time
             {}

bool MockMotionSensor::isMotionDetected() const {
    return distribution(engine);
}