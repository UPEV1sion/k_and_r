#include <stdio.h>

#define MAXLINE 1024

int get_line(char line[], int maxline);

void copy(char *from, char *to);

int main(void)
{
    int len;
    int max;
    char line[MAXLINE];
    char longest[MAXLINE];

    max = 0;
    while ((len = get_line(line, MAXLINE)) > 0)
    {
        if (len > max)
        {
            max = len;
            copy(longest, line);
        }
    }

    if(max > 0) printf("%s\n", longest);

    return 0;
}

void copy(char *from, char *to)
{
    while ((*to++ = *from++));
}

int get_line(char line[], int maxline)
{
    int output_len = 0;
    int input_len = 0;

    int c;
    while((c = getchar()) != EOF && c != '\n')
    {
        if(output_len < maxline - 1) line[output_len++] = c;
        input_len++;
    }

    if(c == '\n')
    {
        if(output_len < maxline - 1) line[output_len++] = c;
        input_len++;
    }

    line[output_len] = 0;

    return input_len;
}