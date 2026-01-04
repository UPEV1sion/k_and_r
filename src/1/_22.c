//
// Created by escha on 27.12.25.
//
#include <stdio.h>
#include <ctype.h>

#define LINELEN 80
#define MAXSIZE 1024

#define MIN(a,b) ((a) < (b) ? (a) : (b))

int get_line(char *buffer, int buffer_size, FILE *file)
{
    int len = 0;

    int c;
    while ((c = fgetc(file)) != EOF && c != '\n' && len < buffer_size - 1)
    {
        buffer[len++] = (char)c;
    }

    buffer[len] = 0;

    return len;
}

int get_last_blank(const char *buffer, const int from, const int to)
{
    int last_pos = -1;

    for (int i = from; i < to; ++i)
    {
        if (isblank(buffer[i])) last_pos = i;
    }

    return last_pos;
}

void fold_buffer(const char *buffer, const int len)
{
    int pos = 0;
    while(pos < len)
    {
        const int end = MIN(pos + LINELEN, len);
        const int last_blank = (end - pos == LINELEN) ?
                               get_last_blank(buffer, pos, end) :
                               -1;
        if(last_blank >= 0)
        {
            printf("%.*s\n", last_blank - pos, buffer + pos);
            pos = last_blank + 1;
        }
        else
        {
            printf("%.*s", LINELEN - 1, buffer + pos);
            if (pos + LINELEN - 1 < len)
                putchar('-');
            putchar('\n');
            pos += LINELEN - 1;
        }
    }
}

void fold_line(void)
{

    char buffer[MAXSIZE];
    int len;
    while ((len = get_line(buffer, sizeof buffer, stdin)) > 0)
    {
        if (len < LINELEN)
            puts(buffer);
        else
            fold_buffer(buffer, len);
    }
}

int main(void)
{
    fold_line();

    return 0;
}