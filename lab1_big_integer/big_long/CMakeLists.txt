add_library(big_long STATIC big_long.cpp big_long_core.cpp big_long_operators.cpp)
target_include_directories(big_long PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)