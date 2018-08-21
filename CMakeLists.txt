cmake_minimum_required(VERSION 3.5.1)
project(LampPost)

SET(COVERAGE OFF CACHE BOOL "Activate code coverage")

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(UNIX OR APPLE)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -g -O0 --coverage")
endif(UNIX OR APPLE)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_definitions(-D_SILENCE_TR1_NAMESPACE_DEPRECATION_WARNING -DZMQ_BUILD_DRAFT_API)

set(gtest_force_shared_crt ON CACHE BOOL "Force generation of shared libraries for GTest." FORCE)
add_subdirectory(3rdparty/googletest)

set(FLATBUFFERS_BUILD_TESTS OFF CACHE BOOL "Enable the build of tests and samples." FORCE)
add_subdirectory(3rdparty/flatbuffers)

set(BUILD_TESTS OFF CACHE BOOL "Whether or not to build zmq tests" FORCE)
set(ZMQ_BUILD_TESTS OFF CACHE BOOL "Build the tests for ZeroMQ" FORCE)
set(ENABLE_DRAFTS ON CACHE BOOL "Enable Draft builds" FORCE)
add_subdirectory(3rdparty/libzmq)

add_subdirectory(3rdparty/json-c)

set(BUILD_TESTING OFF CACHE BOOL "Whether or not to build curl tests" FORCE)
set(USE_MANUAL OFF CACHE BOOL "Whether to build the curl manual" FORCE)
set(BUILD_CURL_EXE OFF CACHE BOOL "Whether to build the curl exe" FORCE)
add_subdirectory(3rdparty/curl)

include_directories(
    include
    ${gtest_SOURCE_DIR}/include
    3rdparty/flatbuffers/include
    3rdparty/libzmq/include
    3rdparty/libzmq/src
    3rdparty/json-c
    ${json-c_BINARY_DIR}/include
    3rdparty/curl/include
    3rdparty/tbb/include)

set(SRC
    src/lamppost/LampPost.cpp
    src/lamppost/plugin/PluginTemplate.cpp
    src/lamppost/plugin/PluginInstance.cpp
    src/lamppost/plugin/PluginManager.cpp
    src/lamppost/Identifiable.cpp
    src/lamppost/Filesystem.cpp
    src/lamppost/messages/Message.cpp
    src/lamppost/messages/DataBase.cpp
    src/lamppost/messages/RawDatagram.cpp
    src/lamppost/messages/Datagram.cpp
    src/lamppost/exceptions/KeyNotFoundException.cpp
    src/lamppost/exceptions/ArgumentNullException.cpp
    src/lamppost/exceptions/IndexOutOfBoundsException.cpp
    src/lamppost/exceptions/OperationFailedException.cpp
    src/lamppost/messages/Data.cpp
    src/lamppost/exceptions/Exception.cpp
    src/lamppost/exceptions/InvalidOperationException.cpp
    src/lamppost/bus/Bus.cpp
    src/lamppost/exceptions/DuplicateKeyException.cpp
    src/lamppost/bus/BusParticipant.cpp
    src/lamppost/bus/Publisher.cpp
    src/lamppost/bus/Subscriber.cpp
    src/lamppost/bus/ActionProvider.cpp
    src/lamppost/bus/ActionConsumer.cpp
    src/lamppost/log/Log.cpp
    src/lamppost/settings/SettingsManager.cpp
    src/lamppost/utilities/Uuid.cpp
    src/lamppost/data/RawBytes.cpp
    src/lamppost/utilities/AntMatcher.cpp)

add_library(${PROJECT_NAME}
    ${SRC})

target_link_libraries(${PROJECT_NAME}
    flatbuffers
    json-c)

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


### Flatbuffers
add_custom_target(GenerateFlatbuffersFiles
    COMMAND flatc --cpp -o "${CMAKE_CURRENT_SOURCE_DIR}/include/lamppost/schemas/" "${CMAKE_CURRENT_SOURCE_DIR}/schemas/FBDatagram.fbs"
    COMMAND flatc --cpp -o "${CMAKE_CURRENT_SOURCE_DIR}/include/lamppost/schemas/" "${CMAKE_CURRENT_SOURCE_DIR}/schemas/FBMessage.fbs")

add_dependencies(${PROJECT_NAME}
    GenerateFlatbuffersFiles)


### Intel TBB
if(UNIX)
  set(tbb_lib_file ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/tbb/tbb_cmake_build/tbb_cmake_build_subdir_debug/tbb_debug.so)
  set(tbb_runtime_file tbb_debug.so)

  set(copy_command cp)
elseif(APPLE)
  set(tbb_lib_file ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/tbb/tbb_cmake_build/tbb_cmake_build_subdir_debug/tbb_debug.so)
  set(tbb_runtime_file tbb_debug.so)

  set(copy_command cp)
elseif(WIN32 OR WIN64)
  set(tbb_lib_file ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/tbb/tbb_cmake_build/tbb_cmake_build_subdir_debug/tbb_debug.lib)
  set(tbb_runtime_file tbb_debug.dll)

  set(copy_command copy)
endif()

set(TBB_COPY_SOURCE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/tbb/tbb_cmake_build/tbb_cmake_build_subdir_debug/${tbb_runtime_file})
set(TBB_COPY_DESTINATION_PATH ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${tbb_runtime_file})

if(WIN32 OR WIN64)
  STRING(REGEX REPLACE "/" "\\\\" TBB_COPY_SOURCE_PATH ${TBB_COPY_SOURCE_PATH})
  STRING(REGEX REPLACE "/" "\\\\" TBB_COPY_DESTINATION_PATH ${TBB_COPY_DESTINATION_PATH})
endif()

add_custom_target(tbb_target
    COMMAND git checkout "${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/tbb/cmake/TBBBuild.cmake"
    COMMAND patch -t "${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/tbb/cmake/TBBBuild.cmake" "${CMAKE_CURRENT_SOURCE_DIR}/patches/tbb/gmake_to_make.patch"
    COMMAND ${CMAKE_COMMAND}
      -DCMAKE_ARCHIVE_OUTPUT_DIRECTORY="${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}"
      -DCMAKE_LIBRARY_OUTPUT_DIRECTORY="${CMAKE_LIBRARY_OUTPUT_DIRECTORY}"
      -DCMAKE_RUNTIME_OUTPUT_DIRECTORY="${CMAKE_RUNTIME_OUTPUT_DIRECTORY}"
      -DCMAKE_CXX_COMPILER="${CMAKE_CXX_COMPILER}"
      -DCMAKE_SYSTEM_NAME="${CMAKE_SYSTEM_NAME}"
      -P "${CMAKE_CURRENT_SOURCE_DIR}/cmake/tbb_build.cmake"
    COMMAND ${copy_command} "${TBB_COPY_SOURCE_PATH}" "${TBB_COPY_DESTINATION_PATH}"
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/tbb")

target_link_libraries(${PROJECT_NAME}
    ${tbb_lib_file})

add_dependencies(${PROJECT_NAME}
    tbb_target)


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

macro(link_lp_plugin pluginname)
  set(libraries ${ARGN})
  target_link_libraries(plugin_${pluginname} ${libraries})
endmacro()

macro(depend_lp_plugin pluginname)
  set(dependencies ${ARGN})
  add_dependencies(plugin_${pluginname} ${dependencies})
endmacro()

add_lp_plugin(sysinfo
    src/plugins/sysinfo/SysInfo.cpp)

add_lp_plugin(link
    src/plugins/link/Link.cpp)

link_lp_plugin(link
    libzmq-static)

add_lp_plugin(downloader
    src/plugins/downloader/Downloader.cpp
    src/plugins/downloader/Curl.cpp)

depend_lp_plugin(downloader
    tbb_target)

link_lp_plugin(downloader
    libcurl)


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
add_lp_test(lamppost/messages MessageTest)
add_lp_test(lamppost/messages RawDatagramTest)
add_lp_test(lamppost/messages DatagramTest)
add_lp_test(lamppost/messages DataTest)
