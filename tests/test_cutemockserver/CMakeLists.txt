cmake_minimum_required(VERSION 3.16)

set(target test_cutemockserver)

set(CMAKE_AUTORCC ON)

set(resources test_resources.qrc)
set(headers test_cutemockserver.h)
set(sources test_cutemockserver.cpp)

add_executable(${target} ${headers} ${sources} ${resources})
target_include_directories(${target} PRIVATE "${PROJECT_SOURCE_DIR}")
target_link_libraries(${target} PRIVATE Qt5::Test PRIVATE Qt5::Network ${PROJECT_NAME})
