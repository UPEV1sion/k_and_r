#include <stdarg.h>
#include <stdio.h>

void minscanf(char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);

    for(char *p = fmt; *p; p++)
    {
        if(*p != '%')
        {
            getchar();
            continue;
        }

        switch(*++p)
        {
            case 'i': scanf("%i", va_arg(ap, int*));      break;
            case 'd': scanf("%d", va_arg(ap, int*));      break;
            case 'o': scanf("%o", va_arg(ap, unsigned*)); break;
            case 'x': scanf("%x", va_arg(ap, unsigned*)); break;
            case 'u': scanf("%u", va_arg(ap, unsigned*)); break;
            case 'c': scanf("%c", va_arg(ap, char*));     break;
            case 's': scanf("%s", va_arg(ap, char*));     break;
            case 'f': scanf("%f", va_arg(ap, float*));    break;
            case 'e': scanf("%e", va_arg(ap, float*));    break;
            case 'g': scanf("%g", va_arg(ap, float*));    break;
            default: break;
        }
    }

    va_end(ap);
}

int main(void)
{

    char buffer[1024];
    int val;
    minscanf("%s", buffer);
    minscanf("%d", &val);

    printf("%s, %d\n", buffer, val);

    return 0;
}
