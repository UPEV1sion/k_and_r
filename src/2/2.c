#include <stdio.h>

int get_line(char s[], int lim)
{
    int i;
    for(i = 0; i < lim - 1; ++i)
    {
        int c;
        if((c = getchar()) == EOF) break;

        s[i] = c;

        if(c == '\n') break;
    }

    s[i] = 0;

    return i;
}

int main(void)
{
    char buffer[80];

    get_line(buffer, sizeof buffer);
    puts(buffer);

    return 0;
}
