add_library(orchestration
        SmartOrchestrator.hpp
        SmartOrchestrator.cpp
        SmartMqttOrchestrator.hpp
        SmartMqttOrchestrator.cpp
)
target_include_directories(orchestration PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/../../mqtt
        ${CMAKE_CURRENT_SOURCE_DIR}/../../utils
)