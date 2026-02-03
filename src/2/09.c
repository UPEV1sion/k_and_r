#include <stdio.h>

int bitcount(unsigned x)
{
    int count = 0;

    for (; x; count++, x &= (x - 1))
        ;

    return count;
}

int main(void)
{
    printf("%d\n", bitcount(3));
    return 0;
}
