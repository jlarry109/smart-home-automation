cmake_minimum_required(VERSION 3.10)
project(edge_device)

set(CMAKE_CXX_STANDARD 20)

# Add motion module
add_subdirectory(motion)

# Add temperature_humidity module
add_subdirectory(temperature_humidity)

# Add light module
add_subdirectory(light)

# Use system-installed libs
find_package(PahoMqttCpp REQUIRED)
find_package(OpenSSL REQUIRED)

# Main app (add all sources here)
add_executable(edge_device
    main.cpp
    ../config/ConfigLoader.cpp
    ../mqtt/PahoMqttClient.cpp
)

# Link libraries
target_link_libraries(edge_device
    motion
    temp_humid
    light
    PahoMqttCpp::paho-mqttpp3
    OpenSSL::SSL
    OpenSSL::Crypto
)

# Include custom headers
target_include_directories(edge_device PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../config 
    ${CMAKE_CURRENT_SOURCE_DIR}/../mqtt
)
