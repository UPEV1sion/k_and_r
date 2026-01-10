#include <stdio.h>

int strindex(const char *s, const char *t)
{
    int last = -1;

    for(int i = 0; s[i]; ++i)
    {
        int j = i;
        int k = 0;
        for(; t[k] && s[j] == t[k]; j++, k++)
            ;
        if (k > 0 && t[k] == 0) last = i;
    }

    return last;
}

int main(void)
{
    printf("%d\n", strindex("abobaaboba", "ba"));

    return 0;
}
