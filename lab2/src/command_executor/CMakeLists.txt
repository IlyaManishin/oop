add_library(command_executor
    STATIC
    executor.cpp
    cmd_commands.cpp
    utils/utils.cpp
    base_commands/core_commands.cpp)

target_link_libraries(command_executor PRIVATE cmd_parser wav_lib)
target_link_libraries(command_executor PRIVATE file_executor)
