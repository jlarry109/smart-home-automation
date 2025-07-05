# This builds the light module (interface + mock + monitor)
add_library(light
        ILightSensor.hpp
        MockLightSensor.hpp
        MockLightSensor.cpp
        LightMonitor.hpp
        LightMonitor.cpp
        LightController.hpp
        LightController.cpp
        LuxLogger.hpp
        LuxLogger.cpp
)
target_include_directories(light PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/../../mqtt
)