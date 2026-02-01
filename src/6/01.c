#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

#define BUFSIZE 1024

int getch();
void ungetch(int);

void comment(void)
{
    int c;
    while((c = getch()) != EOF)
    {
        if(c == '*')
        {
            if((c = getch()) == '/')
            {
                break;
            }
            else
            {
                ungetch(c);
            }
        }
    }
}

int getword(char *word, int lim)
{
    char *w = word;

    int c;
    while(isspace(c = getch()))
        ;

    if(c == EOF) return EOF;

    *w++ = c;

    if(isalpha(c) || c == '_' || c == '#')
    {
        for(; --lim > 0; w++)
        {
            if(!isalnum(*w = getch()) && *w != '_')
            {
                ungetch(*w);
                break;
            }
        }
    }
    else if(c == '\'' || c == '"')
    {
        for(; --lim > 0; w++)
        {
            if((*w = getch()) == '\\')
            {
                if(--lim <= 0) break;
                *++w = getch();
            }
            else if(*w == c)
            {
                w++;
                break;
            }
            else if(*w == EOF)
            {
                break;
            }
        }
    }
    else if(c == '/')
    {
        int d;
        if((d = getch()) == '*')
        {
            comment();
            return getword(word, lim);
        }
        else if(d == '/')
        {
            while((c = getch()) != '\n' && c != EOF);
            return getword(word, lim);
        }
        else
        {
            ungetch(d);
        }
    }

    *w = 0;
    return word[0];
}

int main(void)
{
    char buffer[BUFSIZE];
    while(getword(buffer, sizeof buffer) != EOF)
    {
        puts(buffer);
    }
    return 0;
}

#define STACKSIZE 1024
static int buf[STACKSIZE];
static size_t bufp;

int getch(void)
{
    return (bufp > 0) ? buf[--bufp] : getchar();
}

void ungetch(int c)
{
    if(bufp < STACKSIZE) buf[bufp++] = c;
    else fprintf(stderr, "error: stack overflow!\n");
}

