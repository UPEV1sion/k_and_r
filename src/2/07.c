#include <stdio.h>

unsigned invert(unsigned x, const int p, const int n)
{
    const unsigned mask = ~(~0u << n);
    return x ^ (mask << (p+1-n));
}

int main(void)
{
    printf("%x\n", invert(123, 4, 3));

    return 0;
}
