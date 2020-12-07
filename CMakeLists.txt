cmake_minimum_required(VERSION 2.8)
project(Matrix)

message(STATUS "CMake version: ${CMAKE_VERSION}")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -Wall")
set(CMAKE_BUILD_TYPE "Release")

set(CMAKE_INCLUDE_CURRENT_DIR ON)
aux_source_directory(. source_list)
add_executable(Matrix ${source_list})
find_package(OpenBLAS REQUIRED)
include_directories(${OpenBLAS_INCLUDE_DIRS})
target_link_libraries(Matrix ${OpenBLAS_LIBRARIES})



