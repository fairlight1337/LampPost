cmake_minimum_required(VERSION 3.10)
project(LampPost)

set(CMAKE_CXX_STANDARD 14)

include_directories(
        include
)

add_executable(LampPost src/main.cpp include/lamppost/LampPost.h src/lamppost/LampPost.cpp)