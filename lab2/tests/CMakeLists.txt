add_executable(tests tests.cpp cmd_tests.cpp)

target_link_libraries( tests
    PRIVATE
    cmd_parser
    GTest::gtest
    GTest::gtest_main
)
gtest_add_tests(TARGET tests EXTRA_ARGS --gtest_color=1)
