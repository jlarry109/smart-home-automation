#include "LightMonitor.hpp"
#include "../../utils/Logging.hpp"
#include <chrono>

LightMonitor::LightMonitor(std::shared_ptr <ILightSensor> sensor,
                           std::shared_ptr <LuxLogger> logger,
                           std::shared_ptr <LightController> controller)
                           : sensor_(std::move(sensor)),
                             logger_(std::move(logger)),
                             controller_(std::move(controller)) {}

LightMonitor::~LightMonitor() {
    threadSafeLog("[LightMonitor] Destructor called.");
    stopMonitoring();
}


void LightMonitor::startMonitoring(int intervalMs) {
    if (running_) {
        threadSafeLog("[LightMonitor] Already running");
        return;
    }
    running_ = true;
    monitorThread_ = std::thread(&LightMonitor::monitoringLoop, this, intervalMs);
}

void LightMonitor::stopMonitoring() {
    if (!running_) {
        threadSafeLog("[LightMonitor] Already stopped");
        return;
    }
    running_ = false;
    if (monitorThread_.joinable()) {
        monitorThread_.join();
    }
    monitorThread_ = std::thread();
}

void LightMonitor::monitoringLoop(int intervalMs) {
    try {
        while (running_) {
            float lux = sensor_->readLux();
            logger_->log(lux);
            std::this_thread::sleep_for(std::chrono::milliseconds(intervalMs));
        }
        threadSafeLog("[LightMonitor]: Monitoring stopped gracefully.");
    } catch (const std::exception& e) {
        std::cerr << "[LightMonitor] Uncaught error: " << e.what() << std::endl;
    } catch (...) {
        std::cerr << "[LightMonitor] Unknown fatal error: " << std::endl;
    }
}