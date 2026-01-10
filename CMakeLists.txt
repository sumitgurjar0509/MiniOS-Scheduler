cmake_minimum_required(VERSION 3.10)
project(MiniOS_Scheduler)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)

add_executable(scheduler
    src/main.cpp
    src/scheduler.cpp
    src/utils.cpp
)
