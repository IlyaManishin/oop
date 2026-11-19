add_library(tree_interpreter STATIC
    tree_interpreter.cpp
    rules.cpp

    base_calls/func_calls.cpp
    base_calls/predicates.cpp

    core/types.cpp
    core/utils.cpp
)
