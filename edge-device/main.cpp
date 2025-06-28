#include "motion/MotionMonitor.hpp"
#include "motion/MockMotionSensor.hpp"
#include <memory>
int main() {
    std::shared_ptr<IMotionSensor> mockSensor = std::make_shared<MockMotionSensor>();
    MotionMonitor monitor(mockSensor);
    monitor.startMonitoring(2000); // check every two seconds
    return 0;
}
