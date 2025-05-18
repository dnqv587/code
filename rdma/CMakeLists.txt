cmake_minimum_required(VERSION 3.3)

project(VERBS C CXX )

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif ()

set(CMAKE_CXX_STANDARD 20)
set(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}_DIR}/lib)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)

add_subdirectory(src)