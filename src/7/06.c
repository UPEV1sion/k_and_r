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
        if(fgets(buf1, ARRAY_LEN(buf1), f1) == NULL) break;
        if(fgets(buf2, ARRAY_LEN(buf2), f2) == NULL) break;

        if(strncmp(buf1, buf2, ARRAY_LEN(buf1)) != 0)
        {
            puts("Difference detected!");
            printf("%s:%zu: %s", argv[1], line, buf1);
            printf("%s:%zu: %s", argv[2], line, buf2);
            goto defer;
        }
    }
    
    puts("No difference found!");
    if (ferror(f1)) perror("Error reading first file");
    if (ferror(f2)) perror("Error reading second file");

defer:
    fclose(f1);
    fclose(f2);

    return 0;
}
