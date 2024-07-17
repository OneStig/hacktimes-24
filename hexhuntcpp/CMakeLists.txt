cmake_minimum_required(VERSION 3.28)
project(hexhuntcpp)

set(CMAKE_CXX_STANDARD 20)

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
endif()

add_executable(hexhuntcpp main.cpp
        scoring.cpp)
