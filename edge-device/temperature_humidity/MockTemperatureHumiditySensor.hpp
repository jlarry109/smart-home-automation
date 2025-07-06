#ifndef MOCK_TEMPERATURE_HUMIDITY_SENSOR_HPP
#define MOCK_TEMPERATURE_HUMIDITY_SENSOR_HPP

#include "ITemperatureHumiditySensor.hpp"
#include <random>
#include <chrono>

class MockTemperatureHumiditySensor : public ITemperatureHumiditySensor {
public:
    MockTemperatureHumiditySensor();
    TemperatureHumidityReading read() const override;

private:
    mutable std::default_random_engine engine_;
    mutable std::uniform_real_distribution<float> tempDist_;
    mutable std::uniform_real_distribution<float> humidDist_;
    mutable int callCount_ =  0;
};
#endif // MOCK_TEMPERATURE_HUMIDITY_SENSOR_HPP