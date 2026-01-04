#include <stdio.h>

#define MAXSIZE 1024

size_t strlen(const char *str)
{
    size_t len = 0;
    while(*str++) len++;
    return len;
}

size_t read_line(char *buffer, size_t buffer_size, FILE *fp)
{
    size_t len = 0;

    int c;
    while(len < buffer_size - 1 && (c = fgetc(fp)) != EOF)
    {
        buffer[len++] = (char) c;
        if(c == '\n') break;
    }

    buffer[len] = 0;

    return len;
}

void reverse(char *str)
{
    const size_t len = strlen(str);
    for(size_t i = 0; i < len / 2; ++i)
    {
        const char c = str[i];
        str[i] = str[len - 1 - i];
        str[len - 1 - i] = c;
    }
}

int main(void)
{
    char buffer[MAXSIZE];
    while(read_line(buffer, sizeof buffer, stdin) > 0)
    {
        reverse(buffer);
        puts(buffer);
    }

    return 0;
}