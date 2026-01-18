#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

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

bool error_happened;

int getch(void);
void ungetch(int c);

void declare(void)
{
	while(gettoken() != EOF)
	{
		strcpy(datatype, token);
		out[0]         = 0;
		error_happened = false;
		dcl();

		if(error_happened) continue;

		if(tokentype != '\n') fprintf(stderr, "syntax error\n");
		else printf("%s: %s %s\n", name, out, datatype);
	}
}

void undeclare(void)
{
	int type;
	char temp[2048];

	while(gettoken() != EOF)
	{
		strcpy(out, token);
		while((type = gettoken()) != '\n')
		{
			if(type == PARENS || type == BRACKETS)
			{
				strcat(out, token);
			}
			else if(type == '*')
			{
				sprintf(temp, "(*%s)", out);
				strcpy(out, temp);
			}
			else if(type == NAME)
			{
				sprintf(temp, "%s %s", token, out);
				strcpy(out, temp);
			}
			else
			{
				printf("invalid input at %s\n", token);
			}
		}
		puts(out);
	}
}

int main(int argc, char **argv)
{
	if(argc > 1)
	{
		if(0 == strcmp(argv[1], "-d")) 
		{
			declare();
		}
		else if(0 == strcmp(argv[1], "-u")) 
		{
			undeclare();
		}
		else
		{
			fprintf(stderr, "error: unknown flag: %s\n", argv[1]);
			return 1;
		}
	}
	else 
	{
		declare();
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

void skip_to_end(void)
{
	int c;
	while((c = gettoken()) != EOF && c != '\n')
		;
}

void dcl(void)
{
	int star_count = 0;
	
	while(gettoken() == '*')
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
		if(tokentype != ')')
		{
			fprintf(stderr, "error: missing )\n");
			error_happened = true;
			return;
		}
	}
	else if (tokentype == NAME)
	{
		strcpy(name, token);
	}
	else
	{
		fprintf(stderr, "error: expected name or (dcl)\n");
		error_happened = true;
		return;
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
