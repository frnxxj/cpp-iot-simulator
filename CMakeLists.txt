cmake_minimum_required(VERSION 4.1)
project(simulator)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(include)

find_package(Threads REQUIRED)

set(CMAKE_EXE_LINKER_FLAGS "-static")

add_executable(simulator main.cpp
        include/protocol.hh
        include/ring_buffer.hh)
