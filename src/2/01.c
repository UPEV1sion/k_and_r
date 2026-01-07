#include <stdio.h>
#include <limits.h>
#include <float.h>

void print_digit_ulim(const char *name, const unsigned long max)
{
    printf("unsigned %s: max: %lu\n", name, max);    
}

void print_digit_slim(const char *name, const long min, const long max)
{
    printf("signed %s: min: %ld, max: %ld\n", name, min, max);    
}

void print_float_slim(const char *name, const double min, const double max)
{
    printf("signed %s: min: %lf, max: %lf\n", name, min, max);    
}

void compute_limits(void)
{
    puts("computed");

    const char schar_max = (unsigned char) ~0u >> 1;
    const char schar_min = (char) ~schar_max;
    print_digit_slim("char", schar_min, schar_max);

    const unsigned char uchar_max = ~0;
    print_digit_ulim("char", uchar_max);

    const short sshort_max = (unsigned short) ~0 >> 1;
    const short sshort_min = (short) ~schar_max;
    print_digit_slim("short", sshort_min, sshort_max);

    const unsigned short ushort_max = ~0;
    print_digit_ulim("short", ushort_max);

    const int sint_max = (unsigned int) ~0 >> 1;
    const int sint_min = ~sint_max;
    print_digit_slim("int", sint_min, sint_max);

    const unsigned int uint_max = ~0;
    print_digit_ulim("int", uint_max);

    const long slong_min = (unsigned long) ~0l >> 1l;
    const long slong_max = ~slong_min;
    print_digit_slim("long", slong_min, slong_max);

    const unsigned long ulong_max = ~0l;
    print_digit_ulim("long", ulong_max);
}

void print_limits(void)
{
    puts("limits.h");

    print_digit_slim("char", SCHAR_MIN, SCHAR_MAX);
    print_digit_ulim("char", UCHAR_MAX);

    print_digit_slim("short", SHRT_MIN, SHRT_MAX);
    print_digit_ulim("short", USHRT_MAX);  

    print_digit_slim("int", INT_MIN, INT_MAX);
    print_digit_ulim("int", UINT_MAX);

    print_digit_slim("long", LONG_MIN, LONG_MAX);
    print_digit_ulim("long", ULONG_MAX);

    print_float_slim("float", FLT_MIN, FLT_MAX);
    print_float_slim("double", DBL_MIN, DBL_MAX);
    print_float_slim("long double", LDBL_MIN, LDBL_MAX);
}

int main(void)
{
    print_limits();
    compute_limits();

    return 0;
}