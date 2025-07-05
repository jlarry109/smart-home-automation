#ifndef I_TEMPERATURE_HUMIDITY_SENSOR_HPP
#define I_TEMPERATURE_HUMIDITY_SENSOR_HPP

struct TemperatureHumidityReading {
    float temperatureCelsius;
    float humidityPercent;
};


class ITemperatureHumiditySensor {
public:
    // Reads temperature and humidity as a single atomic reading
    virtual TemperatureHumidityReading read() const = 0;
    virtual ~ITemperatureHumiditySensor() = default;
};
#endif // I_TEMPERATURE_HUMIDITY_SENSOR_HPP