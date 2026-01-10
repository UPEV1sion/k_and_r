#include <stdio.h>


void escape(char *t, const char *s)
{
	while(*s)
	{
		switch(*s)
		{
			case '\a': *t++ = '\\'; *t++ = 'a'; break;
			case '\b': *t++ = '\\'; *t++ = 'b'; break;
			case '\t': *t++ = '\\'; *t++ = 't'; break;
			case '\n': *t++ = '\\'; *t++ = 'n'; break;
			case '\v': *t++ = '\\'; *t++ = 'v'; break;
			case '\f': *t++ = '\\'; *t++ = 'f'; break;
			case '\r': *t++ = '\\'; *t++ = 'r'; break;
			case '\\': *t++ = '\\'; *t++ = '\\'; break;
			default: *t++ = *s; break;
		}
		s++;
	}

	*t = 0;
}

void unescape(char *t, const char *s)
{
	while(*s)
	{
		if(*s == '\\')
		{
			s++;
			if(!*s)
			{
				*t++ = '\\';
				break;
			}

			switch(*s)
			{
				case 'a': *t++ = '\a'; break;
				case 'b': *t++ = '\b'; break;
				case 't': *t++ = '\t'; break;
				case 'n': *t++ = '\n'; break;
				case 'v': *t++ = '\v'; break;
				case 'f': *t++ = '\f'; break;
				case 'r': *t++ = '\r'; break;
				case '\\': *t++ = '\\'; break;
				default: *t++ = '\\'; *t++ = *s; break;
			}
		}
		else 
		{
			*t++ = *s;
		}
		s++;
	}
	*t = 0;
}

int main(void)
{

	char buffer[1024];
	escape(buffer, "\fDas \nist \tein \vtest\b\r");
	puts(buffer);
	unescape(buffer, buffer);
	puts(buffer);
	return 0;
}
