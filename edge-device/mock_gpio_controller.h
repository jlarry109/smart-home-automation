#ifndef MOCK_GPIO_CONTROLLER_H
#define MOCK_GPIO_CONTROLLER_H

#include <iostream>
#include <string>
#include <random>
#include <chrono>
#include <thread>

class MockGpioController {
public:
    MockGpioController() {
        std::cout << "[MockGPIO] Initialized mock GPIO controller." << std::endl;
    }

    void setRelay(bool state) {
        std::string status = state ? "ON" : "OFF";
        std::cout << "[MockGPIO] Relay set to " << status << std::endl;
    }

    std::string readTemperature() {
        // Simulate random temperature between 20 and 30 degrees Celsius
        float temp = 20.0f + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / 10.0f));
        return std::to_string(temp);
    }

    bool detectMotion() {
        // Randomly simulate motion detected
        return rand() % 2 == 0;
    }

    void simulateSensorLoop(int intervalSeconds = 10) {
        while (true) {
            std::string temp = readTemperature();
            bool motion = detectMotion();
            std::cout << "[MockGPIO] Temp: " << temp << "°C, Motion: " << (motion ? "Yes" : "No") << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(intervalSeconds));
        }
    }
};

#endif // MOCK_GPIO_CONTROLLER_H
