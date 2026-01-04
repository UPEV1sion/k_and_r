#include <stdio.h>

#define MAXSIZE 1024
#define MINLEN 80

void copy(char *from, char *to)
{
    while((*to++ = *from++));
}

int get_line(char *line, int maxline)
{
    int outlen = 0, inlen = 0;

    int c;
    while((c = getchar()) != EOF && c != '\n')
    {
        if(inlen < maxline - 1) line[inlen++] = c;
        outlen++;
    }

    if(c == '\n')
    {
        if(inlen < maxline - 1) line[inlen++] = c;
        outlen++;
    }

    line[inlen] = 0;

    return outlen;
}

void print_long_lines(void)
{

    char buffer[MAXSIZE];
    int len = 0;
    while((len = get_line(buffer, MAXSIZE)) > 0)
    {
        if (len > MINLEN) printf("len: %d: %s", len, buffer);
    }
}

int main(void)
{

    print_long_lines();
    return 0;
}