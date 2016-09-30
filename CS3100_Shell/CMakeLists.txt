cmake_minimum_required(VERSION 3.0)
project(CS3100_Shell)

if (CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS}")
endif()

set(SOURCE_FILES
    CS3100_Shell/main.cpp)

add_executable(CS3100_Shell ${SOURCE_FILES})