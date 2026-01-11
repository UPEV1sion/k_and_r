#include <stdio.h>
#include <ctype.h>

#define MAXBUF 1024

static int buf[MAXBUF];
static int bufp;

int getch(void)
{
	return (bufp > 0) ?  buf[--bufp] : getchar();
}

void ungetch(const int c)
{
	if(bufp < MAXBUF) buf[bufp++] = c;
	else fprintf(stderr, "error: too many chars in the buf\n");
}

int getfloat(double *pn)
{
	int c;
	while(isspace(c = getch()))
		;
	if(!isdigit(c) && c != EOF && c != '+' && c != '-')
	{
		ungetch(c);
		return 0;
	}

	double sign = 1.0f;
	if(c == '+' || c == '-') 
	{
		sign = (c == '-') ? -1.0f : 1.0f;
		int next = getch();
		if(!isdigit(next))
		{
			ungetch(next);
			ungetch(c);
			return 0;
		}
		c = next;
	}

	for(*pn = 0; isdigit(c); c = getch())
		*pn = 10 * *pn + (c - '0');

	if(c == '.') c = getch();
	
	double pow = 1.0f;
	for(; isdigit(c); c = getch())
	{
		*pn = 10 * *pn + (c - '0');
		pow *= 10;
	}

	*pn = *pn * sign / pow;
	if(c != EOF) ungetch(c);

	return c;
}

int main(void)
{
	double i;
	if(getfloat(&i) > 0) printf("%f\n", i);
	else fprintf(stderr, "error: invalid num\n");

	return 0;
}

