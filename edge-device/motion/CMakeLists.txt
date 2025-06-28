# This builds the motion module (interface + mock + monitor)
add_library(motion
        IMotionSensor.hpp
        MockMotionSensor.hpp
        MockMotionSensor.cpp
        GPIOMotionSensor.hpp
        GPIOMotionSensor.cpp
        MotionMonitor.hpp
        MotionMonitor.cpp
)
