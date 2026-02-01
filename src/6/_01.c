#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

int getch(void);
int peekch(void);
void ungetch(int c);

int getlit(char *word, int lim)
{
    char *w = word;

    int c;
    while((c = getch()) != EOF && c != '"' && lim > 1)
    {
        if(c == '\\')
        {
            switch(c = getch())
            {
            case 'a': *w++ = '\a'; break;
            case 'b': *w++ = '\b'; break;
            case 't': *w++ = '\t'; break;
            case 'n': *w++ = '\n'; break;
            case 'v': *w++ = '\v'; break;
            case 'f': *w++ = '\f'; break;
            case 'r': *w++ = '\r'; break;
            case '"': *w++ = '"';  break;
            case '\'': *w++ = '\'';  break;
            default:
                *w++ = c;
                break;
            }
        }
		else
		{
			*w++ = c;
		}
		lim--;
    }

	*w = 0;
	return word[0];
}

void trim_comment(void)
{
	int c;
	if((c = getch()) == '/')
	{
		c = getch();
		if(c == '/') 
		{
			while(getchar() != '\n');
		}		
		else if(c == '*') 
		{
			int last = getch();
			while(last != '*' && (c = getch()) != '/') last = c;
			if(getch() != '/') 
			{
				fprintf(stderr, "error: comment is never closed!\n");
				exit(1);
			}
		}
	}
	else
	{
		ungetch(c);
	}

}

int getword(char *word, int lim)
{
	trim_comment();

	int c;
	while(isspace(c = getch()))
		;

    char *w = word;
    if(c != EOF) *w++ = c;

	if(c == '"' || c == '\'') return getlit(word, lim);

    if(!isalpha(c) && c != '_' && c != '#')
    {
        *w = 0;
        return c;
    }

    for(; --lim > 0; w++)
    {
        if(!isalnum(*w = getch()) && *w != '_')
        {
            ungetch(*w);
            break;
        }
    }

    *w = 0;
    return word[0];
}

int main(void)
{
	char buf[1024];
	while(getword(buf, sizeof(buf)) != 0)
	{
		puts(buf);
	}

    return 0;
}

#define MAXBUF 1024
static int buf[MAXBUF];
static size_t bufp;

int getch(void)
{
    return (bufp > 0) ? buf[--bufp] : getchar();
}

void ungetch(int c)
{
    if(bufp >= MAXBUF) fprintf(stderr, "char buffer overflow!\n");
    else buf[bufp++] = c;
}
