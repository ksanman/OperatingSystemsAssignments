cmake_minimum_required(VERSION 3.0)
project(CS3100_ComputePi)

find_package (Threads)

add_executable (CS3100_ComputePi main.cpp)
target_link_libraries (CS3100_ComputePi ${CMAKE_THREAD_LIBS_INIT})

set(CMAKE_CXX_FLAGS "-std=c++11 -lpthread -pthread -O3 ${CMAKE_CXX_FLAGS}")

