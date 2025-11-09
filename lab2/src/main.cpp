#include "command_executor/executor.hpp"

#include <iostream>

int main(int argc, char **argv)
{
    bool res = command_executor::run_from_cmd_args(argc, argv);
    return res ? EXIT_SUCCESS : EXIT_FAILURE;
}