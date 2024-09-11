cmake_minimum_required(VERSION 3.0)

project(MOCK)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

link_directories(./lib)

include_directories(./include)

add_subdirectory(./src)
