set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(TBBBuild)

# Dirty, dirty hack to get local compile to work (as TBB make is a bit picky when it comes to supported compilers).
if(WIN32 OR WIN64)
  if("${CMAKE_CXX_COMPILER}" MATCHES "cl.exe")
    set(CMAKE_CXX_COMPILER "cl")
  endif()

  #message(STATUS "asd ${CMAKE_CURRENT_SOURCE_DIR}/../../cmake/helpers/")
  #find_program(MAKE_TOOL gmake PATHS "${CMAKE_CURRENT_SOURCE_DIR}/../../cmake/helpers/gmake.bat")# HINTS "C:/MingW/MinGW/bin/make.exe")
endif()

#message(STATUS "asdasd ${MAKE_TOOL}")

tbb_build(
    TBB_ROOT "${CMAKE_CURRENT_SOURCE_DIR}"
    #MAKE_TOOL "${MAKE_TOOL}"
    MAKE_ARGS "compiler=${CMAKE_CXX_COMPILER}")
