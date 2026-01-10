#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <string.h>

#define MAXOP 100
#define MAXLINE 1024
#define ALPH_SIZE 26
#define NUMBER '0'
#define NAME 'n'

int getop(char token[]);

void push(double);
double pop(void);
void peek2(void);
void dup2(void);
void swap2(void);
void clear(void);

int get_line(char s[], int lim);

void mathfunc(char []);

int main(void)
{
    double vars[ALPH_SIZE] = {0.0};
    int var = -1;
    double last = 0;

    int type;
    char token[MAXOP];
    while((type = getop(token)) != EOF)
    {
        switch(type)
        {
            case NUMBER: push(atof(token)); break;
            case NAME: mathfunc(token); break;
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
            case 'p': peek2(); break;
            case 'd': dup2(); break;
            case 's': swap2(); break;
            case 'c': clear(); break;
            case '=': vars[var] = pop(); break;
            case '$': printf("\t%.8g\n", last); break;
            case '\n': printf("\t%.8g\n", (last = pop())); break;
            default:
                if(isupper(type))
                {
                    var = type - 'A';
                    push(vars[var]);
                }
                else fprintf(stderr, "error: unknown variable\n");
                break;
        }
    }

    return 0;
}

static char line[MAXLINE];
static int line_pos = 0;

int getop(char token[])
{

    if (line[line_pos] == 0)
    {
        if (get_line(line, MAXLINE) == 0) return EOF;
        line_pos = 0;
    }

    int c;
    while((token[0] = c = line[line_pos++]) == ' ' || c == '\t')
        ;
    token[1] = 0;

    int i = 0;
    if(isalpha(c))
    {
        while(isalpha(token[++i] = c = line[line_pos++]))
            ;
        token[i] = 0;
        if(c != EOF) line_pos--;
        if(strlen(token) > 1) return NAME;
        return *token;
    }

    if(!isdigit(c) && c != '.' && c != '-') return c;

    if(c == '-')
    {
        const int next = line[line_pos++];
        if(!isdigit(next) && next != '.')
        {
            line_pos--;
            return '-';
        }

        token[++i] = c = next;
    }

    if(isdigit(c))
        while(isdigit(token[++i] = c = line[line_pos++]))
            ;
    if(c == '.')
        while(isdigit(token[++i] = c = line[line_pos++]))
            ;
    token[i] = 0;
    if(c != EOF) line_pos--;

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

void peek2(void)
{
    if(sp >= 2) printf("\ttop-1:%.8g top:%.8g\n", val_stack[sp-2], val_stack[sp-1]);
    else fprintf(stderr, "error: not enough elements on the stack to peek\n");
}

void dup2(void)
{

    if(sp > 0) push(val_stack[sp-1]);
    else fprintf(stderr, "error: stack empty, can't duplicate\n");
}

void swap2(void)
{
    if(sp >= 2)
    {
        const double temp = val_stack[sp-1];
        val_stack[sp-1] = val_stack[sp-2];
        val_stack[sp-2] = temp;
    }
    else fprintf(stderr, "error: not enough elements to swap\n");
}

void clear(void)
{
    sp = 0;
}

int get_line(char s[], const int lim)
{
    int i;
    for(i = 0; i < lim - 1; ++i)
    {
        int c;
        if((c = getchar()) == EOF) break;

        s[i] = (char) c;

        if(c == '\n') break;
    }

    s[++i] = 0;

    return i;
}

void mathfunc(char s[])
{
    if(0 == strcmp(s, "sin")) push(sin(pop()));
    else if(0 == strcmp(s, "cos")) push(cos(pop()));
    else if(0 == strcmp(s, "tan")) push(tan(pop()));
    else if(0 == strcmp(s, "exp")) push(exp(pop()));
    else if(0 == strcmp(s, "pow"))
    {
        const double rhs = pop();
        push(pow(pop(), rhs));
    }
    else
    {
        fprintf(stderr, "Math operation not supported\n");
    }
}
