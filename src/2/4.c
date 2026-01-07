#include <stdio.h>
#include <stdbool.h>

#define ALPH_SIZE 256

void squeeze(char *s1, const char *s2)
{

    bool s2_contains[ALPH_SIZE] = {0};
    while(*s2) s2_contains[(unsigned char) *s2++] = true;

    size_t write = 0;

    for(size_t read = 0; s1[read]; ++read)
    {
        if(!s2_contains[(unsigned char) s1[read]]) s1[write++] = s1[read];
    }

    s1[write] = 0;
}

int main(void)
{

    char s1[] = "katze";
    squeeze(s1, "zk");
    puts(s1);

    return 0;
}
