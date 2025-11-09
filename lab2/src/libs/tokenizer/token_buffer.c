#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#include "token_buffer.h"

typedef struct TTokenBuffer
{
    TToken *data;
    int left, right;
    int capacity;
} TTokenBuffer; 

TTokenBuffer *get_token_buf()
{
    TTokenBuffer *buf = (TTokenBuffer *)malloc(sizeof(TTokenBuffer));
    if (buf == NULL)
        return NULL;

    buf->data = (TToken *)malloc(BASE_SIZE * sizeof(TToken));
    if (buf->data == NULL)
    {
        free(buf);
        return NULL;
    }
    buf->left = 0;
    buf->right = 0;
    buf->capacity = BASE_SIZE;
    return buf;
}

int get_buf_length(TTokenBuffer *buf)
{
    int delta = buf->right - buf->left;
    return delta >= 0 ? delta : delta + buf->capacity;
}

static int buf_mod(TTokenBuffer *buf, int value)
{
    return value % buf->capacity;
}

static int increase_buf(TTokenBuffer *buf)
{
    // int new_capacity = buf->capacity * 2;
    // TToken* newData = (TToken *)realloc(buf->data, sizeof(TToken) * new_capacity);
    // if (newData == NULL)
    //     return 0;

    // buf->data = newData;
    // if (buf->left >= buf->right)
    // {
    //     for (int i = 0; i < buf->right; i++)
    //     {
    //         buf->data[buf->capacity + i] = buf->data[i];
    //     }
    //     buf->right = buf->capacity + buf->right;
    // }
    // buf->capacity = new_capacity;
    // return 1;
    int old_cap = buf->capacity;
    int new_cap = old_cap * 2;

    TToken *newData = (TToken *)malloc(sizeof(TToken) * new_cap);
    if (newData == NULL)
        return 0;

    int len = get_buf_length(buf);
    // скопировать в логическом порядке
    for (int i = 0; i < len; ++i)
        newData[i] = buf->data[(buf->left + i) % old_cap];

    free(buf->data);
    buf->data = newData;
    buf->capacity = new_cap;
    buf->left = 0;
    buf->right = len;
    return 1;

}

int append_token_to_buf(TTokenBuffer *buf, TToken token)
{
    if (get_buf_length(buf) == buf->capacity - 1)
    {
        int r = increase_buf(buf);
        if (!r)
            return 0;
    }
    buf->data[buf->right] = token;
    buf->right = buf_mod(buf, buf->right + 1);
    return 1;
}

int pop_tokens_from_buf(TTokenBuffer *buf, int tokensCount)
{
    assert(get_buf_length(buf) >= tokensCount);

    buf->left = buf_mod(buf, buf->left + tokensCount);
    return 1;
}

int get_token_from_buf(TTokenBuffer *buf, TToken *dest, int curPos)
{
    assert(curPos >= 0);
    if (curPos >= get_buf_length(buf))
        return 0;

    int index = buf_mod(buf, buf->left + curPos);
    *dest = buf->data[index];
    return 1;
}

void delete_token_buf(TTokenBuffer *buf)
{
    if (buf == NULL)
        return;

    free(buf->data);
    free(buf);
}