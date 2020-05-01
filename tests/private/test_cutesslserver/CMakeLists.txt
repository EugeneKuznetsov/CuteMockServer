cmake_minimum_required(VERSION 3.16)

set(target test_cutesslserver)

set(headers test_cutesslserver.h)
set(sources test_cutesslserver.cpp)

add_executable(${target} ${headers} ${sources})
target_include_directories(${target} PRIVATE "${PROJECT_SOURCE_DIR}")
target_link_libraries(${target} PRIVATE Qt5::Test PRIVATE Qt5::Network ${PROJECT_NAME}_private)
