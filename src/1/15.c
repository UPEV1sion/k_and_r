#include <stdio.h>

#define STEP 20

float fahr_to_celsius(const float fahr)
{
    return (5.0 / 9.0) * (fahr - 32.0);
}

int main()
{
    const float lower = 0;
    const float upper = 300;

    float fahr = lower;

    printf("Fahr  Celsius\n");
    while(fahr <= upper)
    {
        const float celsius = fahr_to_celsius(fahr);
        printf("%4.0f %6.1f\n", fahr, celsius);
        fahr += STEP;
    }

    return 0;
}
