#include <stdio.h>
#include <ctype.h>

#define MAXLINE 80

int main(void)
{
    size_t pos = 0;    

    int c;
    while((c = getchar()) != EOF)
    {
        if(pos >= MAXLINE)
        {
            printf("\n");
            pos = 0;
        }

        if(isgraph(c))
        {
            pos += printf("%c", c);
        }
        else
        {
            pos += printf("0x%X", c);
            if(c == '\n') 
            {
                pos = 0;
                printf("\n");
            }
        }
    }

    return 0;
}
