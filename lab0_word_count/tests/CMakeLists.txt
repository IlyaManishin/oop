add_executable(word_count_tests
    tests.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../cmd/cmd_reader.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../tables/tables.cpp
)
target_include_directories(word_count_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/..)

file(GLOB TEST_FILES "${CMAKE_CURRENT_SOURCE_DIR}/tests_files/*")
file(COPY ${TEST_FILES} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/tests_files)

add_test(NAME word_count_tests
    COMMAND word_count_tests
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)
