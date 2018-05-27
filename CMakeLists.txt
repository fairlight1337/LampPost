cmake_minimum_required(VERSION 3.9.2)
project(LampPost)

SET(COVERAGE OFF CACHE BOOL "Activate code coverage")

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(UNIX OR APPLE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -g -O0 --coverage")
    link_libraries("-stdlib=libc++")
endif(UNIX OR APPLE)

if(WIN32 OR WIN64)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /bigobj")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
endif(WIN32 OR WIN64)

add_definitions(-D_SILENCE_TR1_NAMESPACE_DEPRECATION_WARNING)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(gtest_force_shared_crt ON CACHE BOOL "Force generation of shared libraries for GTest.")
add_subdirectory(
    3rdparty/googletest)

include_directories(
    include
    ${gtest_SOURCE_DIR}/include)

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
    set(SRC_BIN "${SRC_BIN};src/lamppost/lowlevel/Windows.cpp")
endif(WIN32 OR WIN64)

if(UNIX AND NOT APPLE)
    set(SRC_BIN "${SRC_BIN};src/lamppost/lowlevel/Unix.cpp")
endif(UNIX AND NOT APPLE)

if(APPLE)
    set(SRC_BIN "${SRC_BIN};src/lamppost/lowlevel/Osx.cpp")
endif(APPLE)

add_executable(${PROJECT_NAME}-bin
    ${SRC_BIN})

target_link_libraries(${PROJECT_NAME}-bin
    ${PROJECT_NAME})


### Plugins
macro(add_lp_plugin pluginname)
    set(sourcefiles ${ARGN})
    add_library(plugin_${pluginname} SHARED ${sourcefiles})
    target_link_libraries(plugin_${pluginname} ${PROJECT_NAME})
endmacro()

add_lp_plugin(sysinfo
    src/plugins/sysinfo/SysInfo.cpp)


### Tests
enable_testing()
include(GoogleTest)

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


# ------------------------------------------------------------------------------
# Clang Tidy
# ------------------------------------------------------------------------------

if(ENABLE_CLANG_TIDY)
  find_program(CLANG_TIDY_BIN clang-tidy-4.0)
  find_program(RUN_CLANG_TIDY_BIN run-clang-tidy-4.0.py)

  if(CLANG_TIDY_BIN STREQUAL "CLANG_TIDY_BIN-NOTFOUND")
    message(FATAL_ERROR "unable to locate clang-tidy-4.0")
  endif()

  if(RUN_CLANG_TIDY_BIN STREQUAL "RUN_CLANG_TIDY_BIN-NOTFOUND")
    message(FATAL_ERROR "unable to locate run-clang-tidy-4.0.py")
  endif()

  list(APPEND RUN_CLANG_TIDY_BIN_ARGS
    -clang-tidy-binary ${CLANG_TIDY_BIN}
    -header-filter=.*
    -checks=clan*,cert*,misc*,perf*,cppc*,read*,mode*,-cert-err58-cpp,-misc-noexcept-move-constructor)

  add_custom_target(
    tidy
    COMMAND ${RUN_CLANG_TIDY_BIN} ${RUN_CLANG_TIDY_BIN_ARGS}
    COMMENT "running clang tidy")
endif()
