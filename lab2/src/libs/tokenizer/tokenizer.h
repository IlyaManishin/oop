#pragma once

#include "tokenizer_api.h"

#ifdef __cplusplus
extern "C"
{
#endif

    typedef enum TokenizerSettings
    {
        WHITESPACE_IN_TAB = 4,
        MAX_IDENT_LENGTH = 128,
        MAX_TOKEN_LENGTH = 1024
    } TokenizerSettings;

    typedef enum TokenizerStates
    {
        NEW_LINE_STATE,
        INSIDE_LINE_STATE,
        EOF_STATE,
    } TokenizerStates;

    typedef struct TTokenBuffer TTokenBuffer;

    typedef struct TTokenizer
    {
        const char *start;
        const char *cur;
        const char *curLine; // start of code or after \n
        int lineno;
        const char *end;

        TokenizerStates state;
        int curIndent;
        int newIndent;

        int curBufPos; // offset from left of buffer, need for lookahead
        TTokenBuffer *tokensBuf;

        bool isError;
        TTokenizerError tokError;
    } TTokenizer;

    TToken read_new_token(TTokenizer *tokenizer);
    TToken make_error_token(TTokenizer *tokenizer);
    void set_memory_error(TTokenizer *tokenizer);

#ifdef __cplusplus
}
#endif