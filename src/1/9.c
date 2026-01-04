#include <stdio.h>
#include <stdbool.h>

void cleanup_input(void)
{
    bool space_seen = false;
    int c;
    while((c = getchar()) != EOF)
    {
        if(!space_seen) putchar(c);
        if(c == ' ')
        {
            space_seen = true;
        }
        else
        {
            space_seen = false;
        }
    }
}

int main(void)
{
    cleanup_input();
    return 0;
}