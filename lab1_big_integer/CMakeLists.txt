cmake_minimum_required(VERSION 3.10)

project(big_long
    VERSION 1.0.0
    LANGUAGES CXX
    DESCRIPTION "Arbitrary precision integer arithmetic library"
)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_compile_options(-Wall -Wextra -Wpedantic -g)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin)

enable_testing()

add_subdirectory(big_long)
add_subdirectory(tests)

add_executable(big_integer_demo main.cpp)
target_link_libraries(big_integer_demo PRIVATE big_long)
