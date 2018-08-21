set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(TBBBuild)

# Dirty, dirty hack to get local compile to work (as TBB make is a bit picky when it comes to supported compilers).
if(WIN32 OR WIN64)
  # TBB fails when passing absolute paths.
  if("${CMAKE_CXX_COMPILER}" MATCHES "cl.exe")
    set(CMAKE_CXX_COMPILER "cl")
  endif()
elseif(UNIX)
  # TBB doesn't know g++ (but does know gcc).
  if("${CMAKE_CXX_COMPILER}" MATCHES "g\\+\\+")
    set(CMAKE_CXX_COMPILER "gcc")
  endif()
endif()

tbb_build(
    TBB_ROOT "${CMAKE_CURRENT_SOURCE_DIR}"
    MAKE_ARGS "compiler=${CMAKE_CXX_COMPILER}")
