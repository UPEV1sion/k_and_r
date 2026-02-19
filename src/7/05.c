#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <string.h>

#define MAXOP 100
#define NUMBER '0'

void push(double);
double pop(void);

int getch(void);
void ungetch(int);

int main(void)
{
    char s[MAXOP];
    char c;
    
    while(scanf("%s%c", s, &c) == 2)
    {
        double num;
        if(sscanf(s, "%lf", &num) == 1)
        {
            push(num);
        }
        else if(strlen(s) == 1)
        {
            switch(*s)
            {
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
                default: break;
            }
        }
        else
        {
            fprintf(stderr, "error: unsupported operator: %s\n", s);
        }

        if(c == '\n') printf("\t%.8g\n", pop());
    }
    
    return 0;
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
