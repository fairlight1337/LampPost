cmake_minimum_required(VERSION 3.9.2)
project(LampPost)

set(CMAKE_CXX_STANDARD 14)

if(UNIX OR APPLE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
endif(UNIX OR APPLE)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include_directories(
    include)

set(SRC
    src/lamppost/LampPost.cpp
    src/lamppost/PluginTemplate.cpp
    src/lamppost/PluginInstance.cpp
    src/lamppost/PluginManager.cpp
    src/lamppost/Identifiable.cpp
    src/lamppost/Filesystem.cpp
    src/lamppost/messages/Message.cpp
    src/lamppost/messages/DataBase.cpp
    src/lamppost/messages/Datagram.cpp
    src/lamppost/exceptions/KeyNotFoundException.cpp
    src/lamppost/exceptions/ArgumentNullException.cpp
    src/lamppost/exceptions/IndexOutOfBoundsException.cpp
    src/lamppost/messages/Data.cpp
    src/lamppost/exceptions/Exception.cpp
    src/lamppost/exceptions/InvalidOperationException.cpp
    src/lamppost/bus/Bus.cpp
    src/lamppost/exceptions/DuplicateKeyException.cpp
    src/lamppost/bus/Publisher.cpp
    src/lamppost/bus/Subscriber.cpp)

if(WIN32 OR WIN64)
    set(SRC "${SRC};src/lamppost/lowlevel/Windows.cpp")
endif(WIN32 OR WIN64)

if(UNIX AND NOT APPLE)
    set(SRC "${SRC};src/lamppost/lowlevel/Unix.cpp")
endif(UNIX AND NOT APPLE)

if(APPLE)
    set(SRC "${SRC};src/lamppost/lowlevel/Osx.cpp")
endif(APPLE)

add_library(${PROJECT_NAME}
    ${SRC})

if(WIN32 OR WIN64)
    target_link_libraries(${PROJECT_NAME}
        shlwapi.lib)
endif(WIN32 OR WIN64)

if(UNIX OR APPLE)
    target_link_libraries(${PROJECT_NAME}
        dl)
endif(UNIX OR APPLE)

add_executable(${PROJECT_NAME}-bin
    src/main.cpp)

target_link_libraries(${PROJECT_NAME}-bin
    ${PROJECT_NAME})

add_library(plugin_sysinfo SHARED
    src/plugins/sysinfo/SysInfo.cpp)

target_link_libraries(plugin_sysinfo
    ${PROJECT_NAME})
