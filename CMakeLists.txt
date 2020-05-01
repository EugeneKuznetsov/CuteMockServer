cmake_minimum_required(VERSION 3.16)

project(cutemockserver VERSION 0.1 LANGUAGES CXX)

find_package(Qt5 COMPONENTS Network Test REQUIRED)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

add_subdirectory(lib)
add_subdirectory(tests)
