#include <stdio.h>

#define TABSIZE 8

void detab(void)
{
    size_t pos = 0;
    int c;
    while((c = getchar()) != EOF)
    {
        if(c == '\t')
        {
            const int spaces = TABSIZE - (pos % TABSIZE);
            for(int i = 0; i < spaces; ++i) putchar(' ');
            pos += spaces;
            continue;
        }
        if(c == '\n') pos = 0;
        else ++pos;
        putchar(c);
    }
}

int main(void)
{
    detab();

    return 0;
}
