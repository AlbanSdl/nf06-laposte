cmake_minimum_required(VERSION 3.0.0)
project(evrp VERSION 0.1.0)

include(CTest)
enable_testing()

set(CMAKE_BUILD_TYPE "")
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_C_FLAGS "-Wall")
add_library(evrp SHARED c/main.c)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
