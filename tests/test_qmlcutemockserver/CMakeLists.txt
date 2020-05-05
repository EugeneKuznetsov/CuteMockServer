cmake_minimum_required(VERSION 3.16)

set(target test_qmlcutemockserver)

set(qmltests tst_cutemockserver.qml)
set(headers main.h httpclient.h)
set(sources main.cpp httpclient.cpp)

add_definitions(-DQUICK_TEST_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}")
add_executable(${target} ${headers} ${sources} ${qmltests})
target_include_directories(${target} PRIVATE "${PROJECT_SOURCE_DIR}")
target_link_libraries(${target} PRIVATE Qt5::Network PRIVATE Qt5::QuickTest PRIVATE Qt5::Gui PRIVATE Qt5::Qml PRIVATE cutemockserver)
