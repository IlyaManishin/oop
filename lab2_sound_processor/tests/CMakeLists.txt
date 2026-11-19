add_executable(sound_processor_tests
     main.cpp
     cmd_parser_tests.cpp
     cmd_commands_tests.cpp
     wav_tests.cpp
     file_parser_tests.cpp)

target_link_libraries(sound_processor_tests
     PRIVATE
     cmd_parser
     command_executor
     file_parser
     GTest::gtest
     GTest::gtest_main
)

add_test(NAME sound_processor_tests COMMAND sound_processor_tests)
set_tests_properties(sound_processor_tests PROPERTIES WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")
