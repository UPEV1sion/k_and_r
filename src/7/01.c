#include <stdio.h>
#include <string.h>
#include <ctype.h>

int main(int argc, char **argv)
{
    if(argc <= 1)
    {
        fprintf(stderr, "usage: %s lower|upper\n", argv[0]);
        return 1;
    }

    int (*conv)(int);
    if(strcmp("upper", argv[1]) == 0)
    {
        conv = toupper;
    }
    else if(strcmp("lower", argv[1]) == 0)
    {
        conv = tolower;
    }
    else
    {
        fprintf(stderr, "usage: %s lower|upper\n", argv[0]);
        return 1;
    }

    int c;
    while((c = getchar()) != EOF)
        putchar(conv(c));

    return 0;
}
