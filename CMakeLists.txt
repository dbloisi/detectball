project(detectball)

cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

find_package(OpenCV REQUIRED)

include_directories(${PROJECT_BINARY_DIR})

add_executable (${PROJECT_NAME} detectball.cpp)
target_link_libraries (${PROJECT_NAME} ${OpenCV_LIBS})


