set(INTERNAL_FOLDER internal)

add_library(wav_lib
    STATIC
    wav_file.cpp
    wav_reader.cpp
    ${INTERNAL_FOLDER}/wav_exceptions.cpp
    ${INTERNAL_FOLDER}/wav_utils.cpp)
