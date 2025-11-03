add_subdirectory(cmd_parser)
add_subdirectory(wav)
add_executable(main main.cpp command_executor.cpp)

target_link_libraries(main PRIVATE wav_lib cmd_parser)