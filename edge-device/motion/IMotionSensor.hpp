#ifndef IMOTIONSENSOR_HPP
#define IMOTIONSENSOR_HPP

class IMotionSensor {
public:
    virtual ~IMotionSensor() = default;

    virtual bool isMotionDetected() const = 0;
};
#endif
