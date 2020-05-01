cmake_minimum_required(VERSION 3.16)

set(target ${PROJECT_NAME})

set(headers cutemockserver.h)
set(sources cutemockserver.cpp)

add_library(${target} STATIC ${headers} ${sources})
target_link_libraries(${target} PRIVATE Qt5::Core)
