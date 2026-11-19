cmake_minimum_required(VERSION 3.10)

project(sound_processor
    VERSION 1.0.0
    LANGUAGES C CXX
    DESCRIPTION "WAV audio processor and DSL scripting engine"
)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(GTest REQUIRED)
include(GoogleTest)
enable_testing()

include_directories(src/libs src/include)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin)
add_compile_options(-Wall -Wextra -Wpedantic -O2)

add_subdirectory(src)
add_subdirectory(tests)
