#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>

char* alloc(int n);

#define ITERATIONS 100000
#define MAXLEN 1000

int get_line(char *buf, int bufsize)
{
    char *start = buf;
    int c;    
    while(--bufsize > 0 && (c = getchar()) != EOF)
    {
        *buf++ = c;
        if(c == '\n') break;
    }

    *buf = 0;

    return (int) (buf - start);
}

int alloc_readlines(char *lineptr[], const int maxlines)
{    
    int nlines = 0;
    
    char line[MAXLEN];

    int len;
    while((len = get_line(line, MAXLEN)) > 0)
    {
        char *p;
        if(nlines >= maxlines || (p = alloc(len)) == NULL)
        {
           return -1;
        }
        else
        {
            line[len - 1] = 0;
            strcpy(p, line);
            lineptr[nlines++] = p;
        }
    }

    return nlines; 
}

int buf_readlines(char *lineptr[], char *buf, const int bufsize, const int maxlines)
{
    int nlines = 0;
    int buf_offset = 0;

    char line[MAXLEN];

    int len;
    while((len = get_line(line, MAXLEN)) > 0)
    {
        if(nlines >= maxlines || buf_offset + len > bufsize)
        {
            return -1;
        } 
        else
        {  
            line[len - 1] = 0;
            strcpy(buf + buf_offset, line);
            lineptr[nlines++] = buf + buf_offset;
            buf_offset += len;
        }
    }

    return nlines;
}

#define ALLOCSIZE 10000
static char allocbuf[ALLOCSIZE];
static char *allocp = allocbuf;

char* alloc(const int n)
{
    if(allocbuf + ALLOCSIZE - allocp >= n)
    {
        allocp += n;
        return allocp - n;
    }
    
    return 0;
}

void afree(char *p)
{
    if(p >= allocbuf && p < allocbuf + ALLOCSIZE)
        allocp = p; 
}

void areset(void)
{
    allocp = allocbuf;
}


#define MAXLINES 90
static char *lineptr[MAXLINES];

int main(void)
{
    const clock_t alloc_start = clock();
    for(size_t i = 0; i < ITERATIONS; ++i) 
    {
        alloc_readlines(lineptr, MAXLINES);
        areset();
    }
    const clock_t alloc_stop = clock();
    printf("%f\n", (double) (alloc_stop - alloc_start) / CLOCKS_PER_SEC);

    char buf[8192];
    const clock_t buf_start = clock();
    for(size_t i = 0; i < ITERATIONS; ++i)
    {
         buf_readlines(lineptr, buf, MAXLINES, MAXLINES);
    }
    const clock_t buf_stop = clock();
    printf("%f\n", (double) (buf_stop - buf_start) / CLOCKS_PER_SEC);
       

    return 0;
}

