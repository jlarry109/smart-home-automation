#include "MockTemperatureHumiditySensor.hpp"

MockTemperatureHumiditySensor::MockTemperatureHumiditySensor()
    : engine_(std::chrono::system_clock::now().time_since_epoch().count()),
      tempDist_(22.0f, 28.0f), // Indoor temp
      humidDist_(40.0f, 60.0f) // Reasonable humidity range
{}

TemperatureHumidityReading MockTemperatureHumiditySensor::read() const {
    return {
        tempDist_(engine_),
        humidDist_(engine_)
    };
}
