#include <stdio.h>
#include <string.h>
#include <assert.h>

#define BUFSIZE 1024

#define ARRAY_LEN(a)(sizeof(a)/sizeof(a[0]))

int main(int argc, char **argv)
{
    if(argc < 3) 
    {
        fprintf(stderr, "usage: %s <file 1> <file 2>\n", argv[0]);
        return 1;
    }

    FILE *f1 = fopen(argv[1], "rb");
    FILE *f2 = fopen(argv[2], "rb");
    assert(f1 && "Could not open first file");
    assert(f2 && "Could not open second file");

    char buf1[BUFSIZE];
    char buf2[BUFSIZE];

    for(size_t line = 1;; ++line)
    {
        char *s1 = fgets(buf1, ARRAY_LEN(buf1), f1);
        char *s2 = fgets(buf2, ARRAY_LEN(buf2), f2);
        if(!s1 || !s2) break;

        if(strcmp(buf1, buf2) != 0)
        {
            puts("Difference detected!");
            printf("%s:%zu: %s", argv[1], line, buf1);
            printf("%s:%zu: %s", argv[2], line, buf2);
            goto defer;
        }
    }

    if(!feof(f1) || !feof(f2))
    {
        puts("Files differ in length!");
    }
    else
    {
        puts("No difference found!");
    }

defer:
    fclose(f1);
    fclose(f2);

    return 0;
}

