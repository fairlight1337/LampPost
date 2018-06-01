cmake_minimum_required(VERSION 3.5.1)
project(LampPost)

SET(COVERAGE OFF CACHE BOOL "Activate code coverage")

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(UNIX OR APPLE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -g -O0 --coverage")
endif(UNIX OR APPLE)

add_definitions(-D_SILENCE_TR1_NAMESPACE_DEPRECATION_WARNING)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(gtest_force_shared_crt ON CACHE BOOL "Force generation of shared libraries for GTest." FORCE)
add_subdirectory(
    3rdparty/googletest)

include_directories(
    include
    ${gtest_SOURCE_DIR}/include)

set(SRC
    src/lamppost/LampPost.cpp
    src/lamppost/plugin/PluginTemplate.cpp
    src/lamppost/plugin/PluginInstance.cpp
    src/lamppost/plugin/PluginManager.cpp
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
    src/lamppost/bus/Subscriber.cpp
    src/lamppost/log/Log.cpp
    src/lamppost/config/ConfigurationManager.cpp)

add_library(${PROJECT_NAME}
    ${SRC})

if(WIN32 OR WIN64)
    target_link_libraries(${PROJECT_NAME}
        shlwapi.lib)
endif(WIN32 OR WIN64)

if(UNIX OR APPLE)
    target_link_libraries(${PROJECT_NAME}
        dl
        pthread)
endif(UNIX OR APPLE)

set(SRC_BIN
    src/main.cpp)

if(WIN32 OR WIN64)
    set(SRC_BIN "${SRC_BIN};src/lamppost/platformspecific/windows/Windows.cpp")
endif(WIN32 OR WIN64)

if(UNIX AND NOT APPLE)
    set(SRC_BIN "${SRC_BIN};src/lamppost/platformspecific/xnix/Xnix.cpp")
endif(UNIX AND NOT APPLE)

if(APPLE)
    set(SRC_BIN "${SRC_BIN};src/lamppost/platformspecific/osx/Osx.cpp")
endif(APPLE)

add_executable(${PROJECT_NAME}-bin
    ${SRC_BIN})

target_link_libraries(${PROJECT_NAME}-bin
    ${PROJECT_NAME})


### Windows Service
if(WIN32 OR WIN64)
    add_executable(${PROJECT_NAME}-service
        src/lamppost/platformspecific/windows/service/Service.cpp
        src/lamppost/platformspecific/windows/service/main.cpp)

    target_link_libraries(${PROJECT_NAME}-service
        ${PROJECT_NAME}
        Kernel32.lib
        Advapi32.lib)
endif(WIN32 OR WIN64)


### Plugins
macro(add_lp_plugin pluginname)
    set(sourcefiles ${ARGN})
    add_library(plugin_${pluginname} SHARED ${sourcefiles})
    target_link_libraries(plugin_${pluginname} ${PROJECT_NAME})
endmacro()

add_lp_plugin(sysinfo
    src/plugins/sysinfo/SysInfo.cpp)

add_lp_plugin(link
    src/plugins/link/Link.cpp)


### Tests
enable_testing()

macro(add_lp_test directory testname)
    add_executable(${testname} test/${directory}/${testname}.cpp)

    if(COVERAGE AND UNIX AND NOT APPLE)
        target_compile_options(${testname} PRIVATE --coverage)
        target_link_libraries(${testname} gtest gtest_main ${PROJECT_NAME} --coverage)
    else()
        target_link_libraries(${testname} gtest gtest_main ${PROJECT_NAME})
    endif()

    add_test(${testname} ${CMAKE_BINARY_DIR}/bin/${testname})
endmacro()

add_lp_test(lamppost LampPostConfigurationTest)
add_lp_test(lamppost PluginManagerConfigurationTest)
add_lp_test(lamppost/bus BusTest)
add_lp_test(lamppost/messages DatagramTest)
add_lp_test(lamppost/messages MessageTest)
