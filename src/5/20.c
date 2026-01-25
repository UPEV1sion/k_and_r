#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#define MAXTOKEN 100

#define ARRAY_LEN(a) (sizeof(a)/sizeof(a[0]))

typedef enum { NAME, PARENS, BRACKETS, TYPE, QUALIFIER } TokenType;

void dcl(void);
void dirdcl(void);

int gettoken(void);
int peektoken(void);

static TokenType tokentype;
static char token[MAXTOKEN];
static char name[MAXTOKEN];
static char datatype[MAXTOKEN];
static char out[1000];

static bool error_happened;

// static char* token_type_to_str(const TokenType type)
// {
// 	switch(type)
// 	{
// 		case NAME: return "NAME"; 
// 		case PARENS: return "PARENS"; 
// 		case BRACKETS: return "BRACKETS"; 
// 		case TYPE: return "TYPE"; 
// 		case QUALIFIER: return "QUALIFIER"; 
// 	}
// 	
// 	return "";
// }

static char *types[] = {
	"auto",
	"char",
	"double",
	"float",
	"int",
	"long",
	"short",
	"signed",
	"unsigned",
	"void",
};

static char *qualifiers[] = {
	"const",
	"register",
	"restrict",
	"volatile",
};

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
				if((type = peektoken()) != PARENS && type != BRACKETS)
				{

					sprintf(temp, "*%s", out);
				}
				else
				{
					sprintf(temp, "(*%s)", out);
				}
				strcpy(out, temp);
			}
			else if(type == NAME || type == TYPE || type == QUALIFIER)
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

int str_cmp(const void *a, const void *b)
{
	return strcmp((const char *)a, *(const char * const *)b);
}

char** search_types()
{
	return bsearch(token, types, ARRAY_LEN(types), sizeof(char *), str_cmp);;
}

char** search_qualifiers()
{
	return bsearch(token, qualifiers, ARRAY_LEN(qualifiers), sizeof(char *), str_cmp);
}

static bool peeked;

int peektoken(void)
{
	const int type = gettoken();
	peeked = true;
	return type;
}

int gettoken(void)
{
	if(peeked)
	{
		peeked = false;
		return tokentype;
	}

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

		if(search_types() != NULL) return tokentype = TYPE;
		if(search_qualifiers() != NULL) return tokentype = QUALIFIER;
		
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

void paramdcl()
{

	char temp[1024];
	for (;;)
	{
		gettoken();

		if(tokentype == QUALIFIER)
		{
			snprintf(temp, sizeof(temp), "%s ", token);
			strcat(out, temp);
			gettoken();
		}

		if(tokentype != TYPE)
		{
			error_happened = true;
			fprintf(stderr, "error: expected type in param list\n");
			skip_to_end();
			return;
		}

		strcat(out, token);

		int stars = 0;
		while (gettoken() == '*') stars++;
		while (stars-- > 0) strcat(out, " pointer");

		if (tokentype == ',')
		{
			strcat(out, ", ");
		}
		else if (tokentype == ')')
		{
			break;
		}
		else
		{
			fprintf(stderr, "error: bad param list!\n");
		}
	}
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

	while((type = gettoken()) == PARENS || type == BRACKETS || type == '(')
	{

		if(type == PARENS)
		{
			strcat(out, " function returning");
		}
		else if(type == '(')
		{
			strcat(out, " function expecting ");
			paramdcl();
			strcat(out, " and returning");
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
