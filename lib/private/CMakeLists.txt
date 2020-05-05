cmake_minimum_required(VERSION 3.16)

set(target ${PROJECT_NAME}_private)

set(headers cutesslserver.h cutehttprequest.h cutehttpresponse.h cutehttprouter.h)
set(sources cutesslserver.cpp cutehttprequest.cpp cutehttpresponse.cpp cutehttprouter.cpp)

add_library(${target} OBJECT ${headers} ${sources})
target_link_libraries(${target} PRIVATE Qt5::Network)
