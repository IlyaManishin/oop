#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "syntax_errors.h"

static void print_error_line(const char *errLine, const char *end)
{
    while (*errLine != '\n' &&
           *errLine != '\0' &&
           errLine < end)
    {
        printf("%c", *errLine++);
    }
    printf("\n");
}

void print_error_msg(const char *textMsg)
{
    printf("%s:\n", textMsg);
}

void print_error_with_pos(const char *textMsg, TErrorFilePos pos)
{
    print_error_msg(textMsg);

    int line_pointer_offset = printf("%d line: ", pos.errLineIndex + 1);
    print_error_line(pos.errLineStart, pos.bufferEnd);
    int pointer_offset = line_pointer_offset + (pos.errStart - pos.errLineStart);
    printf("%*c^\n\n", pointer_offset, ' ');
}

TErrorFilePos get_error_file_pos(const char *errStart, const char *errLineStart, int errLineIndex, const char *bufferEnd)
{
    TErrorFilePos pos;
    pos.errStart = errStart;
    pos.errLineStart = errLineStart;
    pos.errLineIndex = errLineIndex;
    pos.bufferEnd = bufferEnd;

    return pos;
}
