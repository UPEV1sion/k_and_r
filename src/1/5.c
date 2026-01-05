#include <stdio.h>

#define STEP 20

int main()
{
    const float lower = 0;
    const float upper = 300;

    float fahr = upper;

    printf("Fahr  Celsius\n");
    while(fahr >= lower)
    {
        const float celsius = (5.0 / 9.0) * (fahr - 32.0);
        printf("%4.0f %6.1f\n", fahr, celsius);
        fahr -= STEP;
    }

    return 0;
}
