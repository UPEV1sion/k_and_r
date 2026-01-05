#include <stdio.h>

#define STEP 20

int main()
{
    const float lower = 0;
    const float upper = 100;

    float celsius = lower;

    printf("Celsius  Fahr\n");
    while(celsius <= upper)
    {
        const float fahr = ((9.0 / 5.0) * celsius) + 32.0;
        printf("%7.0f %6.1f\n", celsius, fahr);
        celsius += STEP;
    }

    return 0;
}
