add_subdirectory(tokenizer)

add_library(file_parser STATIC parser.cpp parser_rules.cpp)
target_link_libraries(file_parser PRIVATE tokenizer)