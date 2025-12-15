#include "../syntax_errors.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

void format_error_with_pos(const char *textMsg, TErrorFilePos pos, char *outBuf, size_t bufSize)
{
    int n = snprintf(outBuf, bufSize, "%s:\n%d line: ", textMsg, pos.errLineIndex + 1);

    size_t lineLen = pos.bufferEnd - pos.errLineStart;
    if (lineLen > bufSize - n - 1)
        lineLen = bufSize - n - 1;
    memcpy(outBuf + n, pos.errLineStart, lineLen);
    n += lineLen;
    outBuf[n++] = '\n';

    int pointer_offset = (pos.errStart - pos.errLineStart) + snprintf(NULL, 0, "%d line: ", pos.errLineIndex + 1);
    if (pointer_offset > (int)(bufSize - n - 1))
        pointer_offset = bufSize - n - 1;
    memset(outBuf + n, ' ', pointer_offset);
    n += pointer_offset;
    outBuf[n++] = '^';
    outBuf[n] = '\0';
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
