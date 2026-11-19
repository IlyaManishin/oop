add_subdirectory(libs)

add_subdirectory(interpreter)
add_subdirectory(file_executor)
add_subdirectory(command_executor)
add_executable(sound_processor main.cpp)

target_link_libraries(sound_processor PRIVATE wav_lib command_executor)