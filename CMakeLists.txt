cmake_minimum_required(VERSION 3.9.2)
project(LampPost)

set(CMAKE_CXX_STANDARD 14)

include_directories(
        include
)

add_executable(LampPost src/main.cpp include/lamppost/LampPost.h src/lamppost/LampPost.cpp)