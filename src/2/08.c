#include <stdio.h>

unsigned rightrot(unsigned x, const int n)
{
    const unsigned mask = ~(~0u << n);
    const unsigned right_bits = x & mask;
    return (x >> n) | (right_bits << (sizeof(unsigned)*8 - n));
}

int main(void)
{
    printf("%u\n", rightrot(123, 1));

    return 0;
}