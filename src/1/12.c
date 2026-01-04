#include <stdio.h>

enum State{IN, OUT};

void words(void)
{
    enum State state = OUT;
    int c;
    while((c = getchar()) != EOF)
    {
        if(c == ' ' || c == '\t' || c == '\n') state = OUT;
        else if(state == OUT)
        {
            putchar('\n');
            state = IN;
        }
        if(state == IN) putchar(c);
    }
    putchar('\n');
}


int main(void)
{
    words();
    return 0;
}