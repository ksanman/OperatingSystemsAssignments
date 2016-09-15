cmake_minimum_required (VERSION 3.0)
project(Assign2)

if (CMAKE_COMPILER_IS_GNUCXX)
	set(CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS}")
endif()

add_executable(Assign2 Fib.h Fib.cpp e.h e.cpp pi.h pi.cpp Main.cpp)
