#include <stdio.h>

#define TABSIZE 8

void entab(void)
{
    int space_count = 0;
    int pos = 0;

    int c;
    while ((c = getchar()) != EOF)
    {
        if (c == ' ')
        {
            space_count++;
            if ((pos + 1) % TABSIZE == 0)
            {
                putchar('\t');
                space_count = 0;
            }
        }
        else
        {
            for(; space_count > 0; space_count--) putchar(' ');

            space_count = 0;
            putchar(c);

            if(c == '\n') pos = -1;
            else if(c == '\t') pos += TABSIZE - (pos % TABSIZE) - 1;
        }

        pos++;
    }
}

int main(void)
{
    entab();
    return 0;
}
