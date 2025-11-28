#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

    typedef enum TokenTypes
    {
        ERROR_TOKEN, // on error

        IDENT,
        NUMBER,
        STRING,

        PLUS,           // +
        MINUS,          // -
        MULTY,          // *
        DIVIS,          // /
        POW,            // **
        DOUBLESLASH,    // //
        PERCENT,        // %
        EQ,             // ==
        NEQ,            // !=
        LT,             // <
        GT,             // >
        LEQ,            // <=
        GEQ,            // >=
        ASSIGN,         // =
        PLUS_ASSIGN,    // +=
        MINUS_ASSIGN,   // -=
        STAR_ASSIGN,    // *=
        SLASH_ASSIGN,   // /=
        PERCENT_ASSIGN, // %=

        LPAREN,   // (
        RPAREN,   // )
        LBRACE,   // {
        RBRACE,   // }
        LBRACKET, // [
        RBRACKET, // ]
        COMMA,    // ,
        DOT,      // .
        COLON,    // :

        INDENT,
        DEDENT,
        NEWLINE,
        EOF_TOKEN
    } TokenTypes;

    typedef struct TToken
    {
        TokenTypes type;

        const char *start;
        const char *end; // for ident only?
        int lineno;
        int col;
    } TToken;

#ifdef __cplusplus
}
#endif