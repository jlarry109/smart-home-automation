#ifndef LIGHT_CONTROLLER_HPP
#define LIGHT_CONTROLLER_HPP

#include <iostream>

/*
 * Takes a lux value and decides whether to turn light on/off (or later, adjust brightness)

Owns relay or LED logic
 * */
class LightController {
public:
    void forceOn();
    void forceOff();

private:
    void turnOn();
    void turnOff();

    bool isLightOn_ = false;
};
#endif // LIGHT_CONTROLLER_HPP
