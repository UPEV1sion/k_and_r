#include <stdio.h>
#include <math.h>
#include <ctype.h>

double atof(const char *s)
{
    while(isspace(*s)) s++;

    const int sign = *s == '-' ? -1 : 1;
    if(*s == '+' || *s == '-') s++;
        
    double val = 0.0;
    double power = 1.0;

    while(isdigit(*s)) 
    {
        val = val * 10 + (*s - '0'); 
        s++;
    }
    if(*s == '.') s++;
    while(isdigit(*s)) 
    {
        val = val * 10 + (*s - '0');
        power *= 10;
        s++;
    }

    val = sign * val / power;

    
    if(tolower(*s) == 'e') val *= pow(10, atof(++s));

    return val;
}

int main(void)
{
    printf("%f\n", atof("123.45e6"));

    return 0;
}

