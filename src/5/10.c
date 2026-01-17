#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include <stdlib.h>

void push(const double f);
double pop(void);

#define NUMBER '0'

int getop(char *buf, char *in)
{
    int c;
    while(isspace(buf[0] = c = *in++))
        ;
    buf[1] = 0;
    if(!isdigit(c) && c != '.' && c != '-') return c;

    int i = 0;
    if(c == '-')
    {
        int next = *in++;
        if(!isdigit(next) && next != '.')
        {
            return '-';
        }
    
        buf[++i] = c = next;
    }

    if(isdigit(c))
        while(isdigit(buf[++i] = c = *in++))
            ;
    if(c == '.')
        while(isdigit(buf[++i] = c = *in++))
            ;
    buf[i] = 0;

    return NUMBER;
}

int main(int argc, char **argv)
{
    char buf[1024];
    while(--argc > 0)
    {
        const int tok = getop(buf, *++argv);
        switch(tok)
        {
            case NUMBER: push(atof(buf)); break;
            case '+': push(pop() + pop()); break;
            case '-': {
                const double rhs = pop();
                push(pop() - rhs);
                break;
            }
            case '*': push(pop() * pop()); break;
            case '/': {
                const double rhs = pop();
                if(rhs == 0.0) fprintf(stderr, "error: division by zero\n");
                else push(pop() / rhs);
                break;
            }
            case '%': {
                const double rhs = pop();
                if(rhs == 0.0) fprintf(stderr, "error: division by zero\n");
                else push(fmod(pop(), rhs));
                break;
            }
            default: fprintf(stderr, "error: unknown operation: %c\n", tok); break;
        }
    }
    
    printf("\t%.8g\n", pop());
    
    return 0;
}

#define VAL_STACK_SIZE 1024
static double val_stack[VAL_STACK_SIZE];
static int valp;

void push(const double f)
{
    if(valp < VAL_STACK_SIZE) val_stack[valp++] = f;
    else fprintf(stderr, "error: could not push, val stack full\n");
}

double pop(void)
{
    if(valp > 0) return val_stack[--valp];
    else fprintf(stderr, "error: could not pop, val stack empty\n");
    return 0.0;
}
