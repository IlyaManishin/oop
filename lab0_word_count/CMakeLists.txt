cmake_minimum_required(VERSION 3.20)

project(word_count
    VERSION 1.0.0
    LANGUAGES CXX
    DESCRIPTION "Word frequency analyzer with CSV export"
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin)

add_compile_options(-Wall -Wextra -g)

add_executable(word_count
    main.cpp
    cmd/cmd_reader.cpp
    tables/tables.cpp
)
target_include_directories(word_count PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

enable_testing()
add_subdirectory(tests)
