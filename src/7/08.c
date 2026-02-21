#include <stdio.h>

#define shift_args(argc, argv)((argc)--, *(argv)++)

#define MAXLINE 1024
#define MAXPAGE 65

int main(int argc, char **argv)
{
    const char *program = shift_args(argc, argv);
    if(argc <= 0)
    {
        fprintf(stderr, "usage: %s <file 1> ... <file n>\n", program);
        return 1;
    }

    char buffer[MAXLINE];
    while(argc > 0)
    {
        const char *path = shift_args(argc, argv);
        FILE *f = fopen(path, "r");
        if(f == NULL)
        {
            fprintf(stderr, "could not open file %s\n", path);
            continue;
        }

        size_t pageno = 1;
        for(size_t lineno = 0; fgets(buffer, MAXLINE, f) != NULL; ++lineno)
        {
            if(lineno % MAXPAGE == 0) printf("\n\fPage %zu - %s\n\n", pageno++, path);
            printf("%s", buffer);
        } 
        fclose(f);
    }

    return 0;
}
