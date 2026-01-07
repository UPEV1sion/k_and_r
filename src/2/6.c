#include <stdio.h>
#include <stdbool.h>

#define ALPH_SIZE 256

unsigned setbits(unsigned x, const int p, const int n, unsigned y)
{
    const unsigned mask = ~(~0 << n);
    x &= ~(mask << (p+1-n));
    y = (y & mask) << (p+1-n);

    return x | y;
}

int main(void)
{
    printf("%u\n", setbits(123, 4, 3, 2));

    return 0;
}
