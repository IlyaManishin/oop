add_subdirectory(cmd_parser)

add_library(command_executor STATIC executor.cpp cmd_commands.cpp)
target_link_libraries(command_executor PRIVATE cmd_parser wav_lib)