#include <stdio.h>

#define MAXSIZE 1024

size_t read_line(char *buffer, size_t buffer_size, FILE *fp)
{
    size_t len = 0;

    int c;
    while(len < buffer_size - 1 && (c = fgetc(fp)) != EOF)
    {
        buffer[len++] = c;
        if(c == '\n') break;
    }

    buffer[len] = 0;

    return len;
}

void strip_trailing(char *buffer, size_t len)
{
    size_t i;
    for(i = len - 1; (buffer[i - 1] == ' ' || buffer[i - 1] == '\t') && i > 0; --i)
        ;

    buffer[i] = '\n';
    buffer[i] = 0;
}

int main(void)
{

    size_t len;
    char buffer[MAXSIZE];
    while((len = read_line(buffer, sizeof buffer, stdin)) > 0)
    {
        strip_trailing(buffer, len);
        puts(buffer);
    }

    return 0;
}