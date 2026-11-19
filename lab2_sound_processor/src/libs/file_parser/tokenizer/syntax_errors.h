#pragma once

#include <stdbool.h>
#include <stdlib.h>

#define CRIT_ERR_MSG_LENGTH 256
#define TOK_ERROR_BUF_SIZE 256

#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct TErrorFilePos
    {
        const char *errStart;
        const char *errLineStart;
        int errLineIndex;
        const char *errLineEnd;
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
    TErrorFilePos get_error_file_pos(const char *errStart,
                                     const char *errLineStart, int errLineIndex,
                                     const char *bufferEnd);
    void format_error_with_pos(const char *textMsg, TErrorFilePos pos, char *outBuf, size_t bufSize);

#ifdef __cplusplus
}
#endif