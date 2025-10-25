cmake_minimum_required(VERSION 3.10)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 23)

project(big_long)

add_compile_options(-Wall -Wextra -g)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_subdirectory(big_long)
add_subdirectory(tests)

add_executable(main main.cpp)
target_link_libraries(main PRIVATE big_long)
