cmake_minimum_required(VERSION 3.16)

set(target ${PROJECT_NAME})
set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_subdirectory(private)

set(headers cutemockserver.h)
set(sources cutemockserver.cpp)

add_library(${target} STATIC ${headers} ${sources})
target_link_libraries(${target} PRIVATE Qt5::Network PRIVATE ${PROJECT_NAME}_private)
