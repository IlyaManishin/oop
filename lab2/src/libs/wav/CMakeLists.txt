set(INTERNAL_FOLDER internal)

add_library(wav_lib
    STATIC
    wav.cpp
    ${INTERNAL_FOLDER}/wav_utils.cpp)
