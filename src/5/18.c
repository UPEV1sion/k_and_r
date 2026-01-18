#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAXTOKEN 100

enum { NAME, PARENS, BRACKETS };

void dcl(void);
void dirdcl(void);

int gettoken(void);

int tokentype;
char token[MAXTOKEN];
char name[MAXTOKEN];
char datatype[MAXTOKEN];
char out[1000];

int getch(void);
void ungetch(int c);

int main(void)
{
	while(gettoken() != EOF)
	{
		strcpy(datatype, token);
		out[0] = 0;
		dcl();
		if(tokentype != '\n') printf("syntax error\n");
		else printf("%s: %s %s\n", name, out, datatype);
	}

	return 0;
}

int gettoken(void)
{
	char *p = token;

	int c;
	while((c = getch()) == ' ' || c == '\t')
		;
	if(c == '(') 
	{
		if((c = getch()) == ')')
		{
			strcpy(token, "()");
			return tokentype = PARENS;
		}
		else
		{
			ungetch(c);
			return tokentype = '(';
		}
	}
	else if(c == '[')
	{
		for(*p++ = c; (*p++ = getch()) != ']'; )
			;
		*p = 0;
		return tokentype = BRACKETS;
	}
	else if(isalpha(c))
	{
		for(*p++ = c; isalnum(c = getch()); )
			*p++ = c;
		*p = 0;
		ungetch(c);
		return tokentype = NAME;
	}
	else
	{
		return tokentype = c;
	}
}

void dcl(void)
{
	int star_count;
	for(star_count = 0; gettoken() == '*'; )
	{
		star_count++;
	}
	dirdcl();
	while(star_count-- > 0)
	{
		strcat(out, " pointer to");
	}
}

void dirdcl(void)
{
	int type;

	if(tokentype == '(')
	{
		dcl();
		if(tokentype != ')') fprintf(stderr, "error: missing )\n");
	}
	else if (tokentype == NAME)
	{
		strcpy(name, token);
	}
	else
	{
		fprintf(stderr, "error: expected name or (dcl)\n");
	}

	while((type = gettoken()) == PARENS || type == BRACKETS)
	{
		if(type == PARENS)
		{
			strcat(out, " function returning");
		}
		else
		{
			strcat(out, " array");
			strcat(out, token);
			strcat(out, " of");
		}
	}
}

#define MAXBUF 1024
static int buf[MAXBUF];
static int bufp;

int getch(void)
{
	return (bufp > 0) ? buf[--bufp] : getchar(); 
}

void ungetch(const int c)
{
	if(bufp < MAXBUF) buf[bufp++] = c;
	else fprintf(stderr, "error: buffer overflow!\n");
}
