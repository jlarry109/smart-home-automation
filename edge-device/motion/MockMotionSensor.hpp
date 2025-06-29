#ifndef MOCK_MOTION_SENSOR_HPP
#define MOCK_MOTION_SENSOR_HPP

#include "IMotionSensor.hpp"
#include <chrono>
#include <random>

class MockMotionSensor : public IMotionSensor {
public:
    MockMotionSensor();

    bool isMotionDetected() const override;

private:
    mutable std::default_random_engine engine;
    mutable std::bernoulli_distribution distribution;
};
#endif // MOCK_MOTION_SENSOR_HPP
