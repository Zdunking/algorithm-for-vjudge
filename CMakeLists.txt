cmake_minimum_required(VERSION 3.20.0)

project(algorithm)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

add_subdirectory(test-lab)
add_subdirectory(chapter1)
add_subdirectory(chapter2)