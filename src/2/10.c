#include <stdio.h>

char lower(const char c)
{
    return (c >= 'A' && c <= 'Z') ? (c - 'A' + 'a') : c;
}

int main(void)
{
    printf("%c\n", lower('C'));

    return 0;
}
