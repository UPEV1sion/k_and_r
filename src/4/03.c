#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>

#define MAXOP 100
#define NUMBER '0'

int getop(char []);

void push(double);
double pop(void);

int getch(void);
void ungetch(int);

int main(void)
{
    int type;
    char s[MAXOP];
    
    while((type = getop(s)) != EOF)
    {
        switch(type)
        {
            case NUMBER: push(atof(s)); break;
            case '+': push(pop() + pop()); break;
            case '*': push(pop() * pop()); break;
            case '-': {
                const double rhs = pop();
                push(pop() - rhs); 
                break;
            }
            case '/': {
                const double rhs = pop();
                if(rhs != 0.0) push(pop() / rhs);
                else fprintf(stderr, "Division by 0 error\n");
                break;
            }
            case '%': {
                const double rhs = pop();
                if(rhs != 0.0) push(fmod(pop(), rhs));
                else fprintf(stderr, "Modulus by 0 error\n");
                break;
            }
            case '\n': printf("\t%.8g\n", pop()); break;
            default: break;
        }
    }
    
    return 0;
}

int getop(char s[])
{
    int c;
    while((s[0] = c = getchar()) == ' ' || c == '\t')
        ;

    s[1] = 0;
    if(!isdigit(c) && c != '.' && c != '-') return c;

    int i = 0;
    if(c == '-') 
    {
        int next = getch();
        if(!isdigit(next) && next != '.')
        {
            ungetch(next);
            return '-';
        }

        s[++i] = c = next;
    }

    if(isdigit(c))
        while(isdigit(s[++i] = c = getch()))
            ;
    if(c == '.')
        while(isdigit(s[++i] = c = getch()))
            ;
    s[i] = 0;
    if(c != EOF) ungetch(c);

    return NUMBER;
}

#define MAXVAL 100

static int sp = 0;
static double val_stack[MAXVAL];

void push(const double f)
{
    if(sp < MAXVAL) val_stack[sp++] = f;
    else fprintf(stderr, "error: stack full, can't push\n");
}

double pop(void)
{
    if(sp > 0) return val_stack[--sp];
    else fprintf(stderr, "error: stack empty, can't pop\n");
    return 0.0;
}

#define BUFSIZE 100

static char buf[BUFSIZE];
static int bufp = 0;

int getch(void)
{
    return (bufp > 0) ? buf[--bufp] : getchar();
}

void ungetch(const int c)
{
    if(bufp >= BUFSIZE) fprintf(stderr, "error: too many chars\n");
    else buf[bufp++] = c;
}
