cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

project(wav_lib)

find_package(GTest REQUIRED)
include(GoogleTest)
enable_testing()

include_directories(src/libs)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
# add_compile_options(-Wall -Wextra -Wpedantic -g -02)
add_compile_options(-Wall -Og)

add_subdirectory(src)
add_subdirectory(tests)
