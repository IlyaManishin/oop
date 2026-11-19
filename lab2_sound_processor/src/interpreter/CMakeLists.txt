add_subdirectory(tree_interpreter)

add_library(interpreter STATIC interpreter.cpp)
target_link_libraries(interpreter PRIVATE tree_interpreter)


