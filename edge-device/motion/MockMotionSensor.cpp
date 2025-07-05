#include "MockMotionSensor.hpp"

MockMotionSensor::MockMotionSensor()
    : engine(std::chrono::system_clock::now().time_since_epoch().count()),
      distribution(0.3), // Bernoulli distribution that returns true ~30% of the time
      callCount_(0) {}

bool MockMotionSensor::isMotionDetected() const {
    // Simulate motion spikes: higher chance of motion every N=20 calls
    callCount_++;

    if (callCount_ % 20 == 0) {
        std::bernoulli_distribution spikeDist(0.7);
        return spikeDist(engine);
    }
    return distribution(engine); // 30% chance otherwise
}