cmake_minimum_required(VERSION 3.9.2)
project(LampPost)

set(CMAKE_CXX_STANDARD 14)

include_directories(
    include
)

set(SRC
    src/main.cpp
    src/lamppost/LampPost.cpp
    src/plugins/sysinfo/SysInfo.cpp
    src/lamppost/PluginTemplate.cpp
    src/lamppost/PluginInstance.cpp
    src/lamppost/PluginManager.cpp
    src/lamppost/Identifiable.cpp
    src/lamppost/Filesystem.cpp)

if(WIN32 OR WIN64)
    set(SRC "${SRC};src/lamppost/lowlevel/Windows.cpp")
endif(WIN32 OR WIN64)

if(UNIX AND NOT APPLE)
    set(SRC "${SRC};src/lamppost/lowlevel/Unix.cpp")
endif(UNIX AND NOT APPLE)

if(APPLE)
    set(SRC "${SRC};src/lamppost/lowlevel/Osx.cpp")
endif(APPLE)

add_executable(LampPost
    ${SRC})

if(WIN32 OR WIN64)
    target_link_libraries(LampPost
        shlwapi.lib)
endif(WIN32 OR WIN64)