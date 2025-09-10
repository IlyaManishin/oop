cmake_minimum_required(VERSION 3.20)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

project(lab0)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
add_compile_options(-g)

add_executable(main main.cpp cmd_settings.cpp)

add_executable(tests tests.cpp cmd_settings.cpp)
add_test(NAME tests COMMAND tests)
