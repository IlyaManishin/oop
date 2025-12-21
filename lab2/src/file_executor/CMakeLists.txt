add_library(file_executor STATIC
    file_executor.cpp
    tree_executor/executor.cpp)

target_link_libraries(file_executor PRIVATE file_parser)
