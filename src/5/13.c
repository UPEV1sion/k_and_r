#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define LINE_COUNT 10
#define BUFSIZE 1024

int get_line(char *buf, int maxline)
{
    char *start = buf;
    
    int c;
    while(--maxline > 0 && (c = getchar()) != EOF)
    {
        *buf++ = c;
        if(c == '\n') break;
    }

    *buf = 0;
    
    return (int) (buf - start);
}

char* strdup(char *str)
{
    size_t len = strlen(str);
    char *dup = malloc(len + 1);
    assert(dup && "Out of mem!");
    strcpy(dup, str);

    return dup;
}

void tail(const int number_lines)
{
    char **lines = calloc(number_lines, sizeof(char *));
    assert(lines && "Out of mem!");

    int line_index = 0;

    char buf[BUFSIZE];
    int len;
    while((len = get_line(buf, BUFSIZE)) > 0)
    {
        char *cur_line = lines[line_index % number_lines];
        free(cur_line);
        lines[line_index % number_lines] = strdup(buf);
        line_index++;
    }
    
    for(int i = 0; i < number_lines; i++) {
        int idx = line_index % number_lines;
        if(lines[idx]) printf("%s", lines[idx]);
        free(lines[idx]);
        line_index++;
    }

    free(lines);
}

int main(int argc, char **argv)
{
    int number_lines = LINE_COUNT;
    if(argc > 1 && *argv[1] == '-') number_lines = atoi(argv[1] + 1);
    
    tail(number_lines);

    return 0;
}
