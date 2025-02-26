cmake_minimum_required(VERSION 2.8.0...3.5.0)
project(zookeeper)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Debug)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
add_compile_options(-Wall -DTHREADED)

include_directories(${PROJECT_SOURCE_DIR}/3rd/zk/include)
link_directories(${PROJECT_SOURCE_DIR}/3rd/zk/lib)

add_subdirectory(src)

