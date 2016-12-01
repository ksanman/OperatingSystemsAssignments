cmake_minimum_required(VERSION 3.0)
project(Assignment6)

find_package(Threads)

set(CMAKE_CXX_FLAGS "-std=c++11 -lpthread -pthread -O3 ${CMAKE_CXX_FLAGS}")

add_executable (CS3100_ASSIGN6 main.cpp)
target_link_libraries (CS3100_ASSIGN6 ${CMAKE_THREAD_LIBS_INIT})
