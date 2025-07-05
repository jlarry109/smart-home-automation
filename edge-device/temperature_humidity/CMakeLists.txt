# This builds the temp-humid module (interface + mock + monitor)
add_library(temp_humid
        ITemperatureHumiditySensor.hpp
        MockTemperatureHumiditySensor.hpp
        MockTemperatureHumiditySensor.cpp
        EnvironmentMonitor.hpp
        EnvironmentMonitor.cpp
)
target_include_directories(temp_humid PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/../../mqtt
)
