#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>

#define MAXLINE 1000

#define shift_args(argc, argv)((argc)--, *(argv)++)

int get_line(char *line, int max, FILE *f);

int match_file(FILE *f, const char *path, const char *pattern, const bool except, const bool number)
{
    char line[MAXLINE];
    long lineno = 0;
    int found = 0; 
    
    while(get_line(line, MAXLINE, f) > 0)
    {
        lineno++;
        if((strstr(line, pattern) != NULL) != except)
        {
            if(f != stdin) printf("%s:", path); 
            if(number) printf("%ld:", lineno);
            printf("%s", line);
            found++;
        }
    }

    return found;
}

int main(int argc, char *argv[])
{
    bool except = false;
    bool number = false;
    int found   = 0;
    char *program = shift_args(argc, argv);

    while(argc > 0 && (*argv)[0] == '-')
    {
        int c;
        while((c = *++argv[0]))
        {
            switch(c)
            {
                case 'x': except = true; break;
                case 'n': number = true; break;
                default: 
                    printf("find: illegal option %c\n", c);
                    argc = 0;
                    found = -1;
                    break;
            }
        }
        shift_args(argc, argv);
    }

    if(argc < 1)
    {
        printf("Usage: %s -x -n <pattern> (<file 1> ... <file n>)\n", program);
    }
    else if(argc == 1)
    {
        const char *pattern = shift_args(argc, argv);
        found += match_file(stdin, NULL, pattern, except, number);
    }
    else
    {
        const char *pattern = shift_args(argc, argv);
        while(argc > 0)
        {
            const char *path = shift_args(argc, argv);
            FILE *f = fopen(path, "r");
            if(f == NULL)
            {
                fprintf(stderr, "find: cannot open %s\n", path);
                continue;
            }
            found += match_file(f, path, pattern, except, number);
            fclose(f);
        }
    }

    return found;
}

int get_line(char *line, const int max, FILE *f)
{
    if(fgets(line, max, f) == NULL)
    {
        return 0;
    }
    return strlen(line);
}
