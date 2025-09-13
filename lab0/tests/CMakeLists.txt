
add_executable(tests tests.cpp ${SRC}/cmd/cmd_reader.cpp ${SRC}/tables/tables.cpp)
add_test(NAME tests COMMAND tests)

file(GLOB TEST_FILES DIRECTORY "tests_files/*")
file(COPY ${TEST_FILES} DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/tests_files)
