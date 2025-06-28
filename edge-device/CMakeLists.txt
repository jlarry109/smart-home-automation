cmake_minimum_required(VERSION 3.10)
project(edge_device)

set(CMAKE_CXX_STANDARD 20)

# Add motion module
add_subdirectory(motion)

# Main application
add_executable(edge_device main.cpp)
target_link_libraries(edge_device motion)
