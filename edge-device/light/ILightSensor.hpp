#ifndef ILIGHT_SENSOR_HPP
#define ILIGHT_SENSOR_HPP



class ILightSensor {
public:
    virtual ~ILightSensor() = default;

    // Reads ambient light in lux (0.0 = total darkness, 1000+ = bright daylight)
    virtual float readLux() const = 0;
};
#endif // ILIGHT_SENSOR_HPP
