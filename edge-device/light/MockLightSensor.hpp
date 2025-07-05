#ifndef MOCK_LIGHT_SENSOR_HPP
#define MOCK_LIGHT_SENSOR_HPP

#include "ILightSensor.hpp"

#include <random>
#include <chrono>

class MockLightSensor : public ILightSensor {
public:
    MockLightSensor();

    float readLux() const override;

private:
    mutable std::default_random_engine engine_;
    mutable std::uniform_real_distribution<float> dist_;
};
#endif // MOCK_LIGHT_SENSOR_HPP