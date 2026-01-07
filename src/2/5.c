#include <stdio.h>
#include <stdbool.h>

#define ALPH_SIZE 256

int any(const char *s1, const char *s2)
{

    bool s2_contains[ALPH_SIZE] = {0};
    while(*s2) s2_contains[(unsigned char) *s2++] = true;

    for (size_t i = 0; s1[i]; ++i)
    {
        if(s2_contains[(unsigned char) s1[i]]) return i;
    }

    return -1;
}

int main(void)
{

    printf("Position of any \"%s\" in \"%s\": %d\n", "katze", "et", any("katze", "et"));

    return 0;
}
