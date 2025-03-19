cmake_minimum_required(VERSION 3.28)
project(kafka)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Debug)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
link_directories(${PROJECT_SOURCE_DIR}/3rd/rdkafka/lib)
include_directories(${PROJECT_SOURCE_DIR}/3rd/rdkafka/include)

add_subdirectory(src)
