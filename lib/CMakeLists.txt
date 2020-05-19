cmake_minimum_required(VERSION 3.16)

set(target ${PROJECT_NAME})

add_subdirectory(private)

set(resources certificates.qrc)
set(headers cutemockserver.h cutefile.h)
set(sources cutemockserver.cpp cutefile.cpp)

add_library(${target} STATIC ${headers} ${sources} ${resources})
target_include_directories(${target}
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
    INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)
target_link_libraries(${target} PRIVATE Qt5::Qml PRIVATE Qt5::Network PRIVATE ${PROJECT_NAME}_private)
