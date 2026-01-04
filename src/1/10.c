#include <stdio.h>

void escape_control(void)
{
    int c;
    while((c = getchar()) != EOF)
    {
        switch (c)
        {
            case '\t': printf("\\t");   break;
            case '\b': printf("\\b");   break;
            case '\\': printf("\\\\");  break;
            default:   putchar(c);             break;
        }
    }
}

int main(void)
{

    escape_control();
    return 0;
}