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
target_include_directories(motion PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../../mqtt
)
