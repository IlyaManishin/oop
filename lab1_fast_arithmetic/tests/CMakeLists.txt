find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

add_executable(big_long_tests program.cpp init_tests.cpp operators_test.cpp)

target_link_libraries(big_long_tests PRIVATE big_long GTest::gtest GTest::gtest_main pthread)

enable_testing()
add_test(NAME BigLongTest COMMAND ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/big_long_tests)