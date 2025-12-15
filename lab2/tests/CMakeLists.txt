add_executable(tests
     main.cpp
     cmd_tests.cpp
     wav_tests.cpp
     file_parser_tests.cpp)

target_link_libraries(tests
     PRIVATE
     cmd_parser
     command_executor
     file_parser
     GTest::gtest
     GTest::gtest_main
)
gtest_add_tests(TARGET tests EXTRA_ARGS --gtest_color=1)
add_test(NAME all_tests COMMAND tests)

file(COPY ./tests_data
     DESTINATION ${CMAKE_BINARY_DIR}/tests)


