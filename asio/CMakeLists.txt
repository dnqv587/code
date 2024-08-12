cmake_minimum_required (VERSION 3.0)

project(ASIO_TEST)
include_directories(${PROJECT_SOURCE_DIR}/include)
#动态库文件输出目录
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
# 静态库文件输出目录
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
#可执行文件输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

set(CMAKE_CXX_STANDARD  20)

add_subdirectory(lessons)