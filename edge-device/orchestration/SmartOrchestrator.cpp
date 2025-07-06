#include "SmartOrchestrator.hpp"
#include <thread>

SmartOrchestrator::SmartOrchestrator(std::shared_ptr <ILightSensor> lightSensor,
                                     std::shared_ptr <IMotionSensor> motionSensor,
                                     std::shared_ptr <ITemperatureHumiditySensor> tempHumiditySensor,
                                     std::shared_ptr <SmartRuleEngine> ruleEngine)
                                     : lightSensor_(std::move(lightSensor)),
                                       motionSensor_(std::move(motionSensor)),
                                       tempHumiditySensor_(std::move(tempHumiditySensor)),
                                       ruleEngine_(std::move(ruleEngine)) {}


void SmartOrchestrator::run(int intervalMs) {
    while (true) {
        float lux = lightSensor_->readLux();
        bool motion = motionSensor_->isMotionDetected();
        TemperatureHumidityReading reading = tempHumiditySensor_->read();

        ruleEngine_->evaluate(lux, reading.temperatureCelsius, reading.humidityPercent, motion);

        std::this_thread::sleep_for(std::chrono::milliseconds(intervalMs));
    }
}