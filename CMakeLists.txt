cmake_minimum_required(VERSION 3.20)

project(nsu_oop_labs
    VERSION 1.0.0
    LANGUAGES C CXX
    DESCRIPTION "Novosibirsk State University - Object-Oriented Programming (C++) Solutions"
)

# Global compiler configuration
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Enable testing globally
include(CTest)
enable_testing()

# Subprojects
add_subdirectory(lab0_word_count)
add_subdirectory(lab1_big_integer)
add_subdirectory(lab2_sound_processor)
