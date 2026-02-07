#include <stdarg.h>
#include <stdio.h>

void minprintf(char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);

    for(char *p = fmt; *p; p++)
    {
        if(*p != '%')
        {
            putchar(*p);
            continue;
        }

        switch(*++p)
        {
            case 'i': // fallthrough
            case 'd': printf("%d", va_arg(ap, int)); break;
            case 'o': printf("%o", va_arg(ap, int)); break;
            case 'x': printf("%x", va_arg(ap, int)); break;
            case 'X': printf("%X", va_arg(ap, int)); break;
            case 'u': printf("%u", va_arg(ap, unsigned)); break;
            case 'c': printf("%c", va_arg(ap, int)); break;
            case 's': for(char *sval = va_arg(ap, char *); *sval; sval++) putchar(*sval); break;
            case 'f': printf("%f", va_arg(ap, double)); break;
            case 'e': printf("%e", va_arg(ap, double)); break;
            case 'E': printf("%E", va_arg(ap, double)); break;
            case 'g': printf("%g", va_arg(ap, double)); break;
            case 'G': printf("%G", va_arg(ap, double)); break;
            case 'p': printf("%p", va_arg(ap, void *)); break;
            default: putchar(*p); break;
        }
    }

    va_end(ap);
}

int main(void)
{
    minprintf("%s", "Hello, World");
    minprintf("%d", 69);
    minprintf("%x", 420);

    return 0;
}
