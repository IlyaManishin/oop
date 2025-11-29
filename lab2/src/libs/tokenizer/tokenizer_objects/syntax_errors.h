#pragma once

#include <stdbool.h>

#ifdef __cplusplus
extern "C"
{
#endif

#define CRIT_ERR_MSG_LENGTH 256

    typedef struct TErrorFilePos
    {
        const char *errStart;
        const char *errLineStart;
        int errLineIndex;
        const char *bufferEnd;
    } TErrorFilePos;

    typedef enum TokErrorTypes
    {
        INVALID_TOKEN,
        INVALID_RULE,
        MEMORY_ERROR,
        WARNING
    } TokErrorTypes;

    typedef struct TTokenizerError
    {
        TokErrorTypes type;
        const char *textMsg;

        bool withPos;
        TErrorFilePos pos;
    } TTokenizerError;

    void print_error_msg(const char *textMsg);
    void print_error_with_pos(const char *errMsg, TErrorFilePos errorPosInfo);
    TErrorFilePos get_error_file_pos(const char *errStart, const char *errLineStart, int errLineIndex, const char *bufferEnd);

#ifdef __cplusplus
}
#endif