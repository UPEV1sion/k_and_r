#include <stdio.h>

#define swap(t,x,y) \
    t _temp = x;    \
    x = y;          \
    y = _temp;

// Bonus :)
#define swap_no_inter(x,y) \
    x = x ^ y;             \
    y = x ^ y;             \
    x = x ^ y;

int main(void)
{

    int c = 42;
    int d = 31;

    printf("c: %d d: %d\n", c, d);
    swap(int, c, d);
    printf("c: %d d: %d\n", c, d);
    swap_no_inter(c,d);
    printf("c: %d d: %d\n", c, d);

    return 0;
}
