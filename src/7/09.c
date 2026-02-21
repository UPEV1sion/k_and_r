#include <stdio.h>
#include <stdbool.h>

#define is_upper_def(c)(c >= 'A' && c <= 'Z')

bool is_upper_func(const char c)
{
    return c >= 'A' && c <= 'Z';
}

int main(void)
{
    printf("%s\n", is_upper_def('T') ? "true" : "false");
    printf("%s\n", is_upper_func('T') ? "true" : "false");
    return 0;
}
