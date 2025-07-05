#include "LightMonitor.hpp"
#include "utils/Logging.hpp"
#include <chrono>

LightMonitor::LightMonitor(std::shared_ptr <ILightSensor> sensor,
                           std::shared_ptr <LuxLogger> logger,
                           std::shared_ptr <LightController> controller)
                           : sensor_(std::move(sensor)),
                             logger_(std::move(logger)),
                             controller_(std::move(controller)) {}

LightMonitor::~LightMonitor() {
    THREAD_SAFE_COUT("[LightMonitor] Destructor called.");
    stopMonitoring();
}


void LightMonitor::startMonitoring(int intervalMs) {
    stopFlag_ = false;
    std::thread(&LightMonitor::monitoringLoop, this, intervalMs);
}

void LightMonitor::stopMonitoring() {
    stopFlag_ = true;
    if (monitorThread_.joinable()) {
        monitorThread_.join();
    }
}

void LightMonitor::monitoringLoop(int intervalMs) {
    try {
        while (!stopFlag_) {
            float lux = sensor_->readLux();
            logger_->log(lux);
            logger_->maybeAlert(lux);
            controller_->update(lux);
            std::this_thread::sleep_for(std::chrono::milliseconds(intervalMs));
        }
        THREAD_SAFE_COUT("[LightMonitor]: Monitoring stopped gracefully.");
    } catch (const std::exception& e) {
        std::cerr << "[LightMonitor] Uncaught error: " << e.what() << std::endl;
    } catch (...) {
        std::cerr << "[LightMonitor] Unknown fatal error: " << std::endl;
    }
}