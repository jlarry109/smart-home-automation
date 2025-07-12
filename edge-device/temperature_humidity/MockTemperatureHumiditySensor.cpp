#include "MockTemperatureHumiditySensor.hpp"

MockTemperatureHumiditySensor::MockTemperatureHumiditySensor()
    : engine_(std::chrono::system_clock::now().time_since_epoch().count()),
      tempDist_(22.0f, 28.0f), // Indoor temp
      humidDist_(40.0f, 60.0f), // Reasonable humidity range
      callCount_(0)
{}

TemperatureHumidityReading MockTemperatureHumiditySensor::read() const {
    callCount_++;

    // Simulate spikes every 20 calls
    float tempSpike = 0.0f;
    float humidSpike = 0.0f;

    if (callCount_ % 20 == 0) {
        tempSpike = 10.0f * (std::bernoulli_distribution(0.7)(engine_) ? 1.0f : -1.0f);
        humidSpike = 20.0f * (std::bernoulli_distribution(0.8)(engine_) ? 1.0f : -1.0f);
    }

    float temperature = tempDist_(engine_) + tempSpike;
    float humidity = humidDist_(engine_) + humidSpike;

    // Clamp humidity to 0-100 range
    humidity = std::max(0.0f, std::min(100.0f, humidity));

    return {
        temperature,
        humidity
    };
}
