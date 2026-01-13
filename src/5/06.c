#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define abs(a) ((a < 0) ? -(a) : (a))
#define NUMBER '0'

size_t get_line(char *s, size_t maxline)
{
    char *start = s;

    int c;
    while((c = getchar()) != EOF && --maxline > 0)
    {
        *s++ = c;
        if(c == '\n') break;
    }

    *s = 0;

    return s - start;
}

void reverse(char *start)
{
    for(char *end = start + strlen(start) - 1; start < end; start++, end--)
    {
        const char c = *start;
        *start = *end;
        *end = c;
    }
}

int strindex(const char *s, const char *t)
{
    const char *start = s;
    while(*s)
    {
        if(*s == *t)
        {
            const char *tt = t;
            const char *ts = s;

            while(*tt && *ts == *tt)
            {
                tt++;
                ts++;
            }
            
            if(*tt == 0) return (int) (s - start);
        }
        s++;
    }

    return -1;
}

int atoi(const char *s)
{
    while(isspace(*s)) s++;

    int sign = (*s == '-') ? -1 : 1;
    if(*s == '-' || *s == '+') s++;

    int res = 0;
    while(isdigit(*s))
    {
        res = (res * 10) + *s - '0';
        s++;
    }

    return res * sign;
}

char* itoa(int num, char *s, const int base)
{
    char *start = s;

    static const char conv[] = "0123456789abcdefghijklmnopqrstuvwxyz";
    
    int sign = num;
    
    do
    {   
        *s++ = conv[abs(num % base)];
    } while(num /= base);    

    if(sign < 0) *s++ = '-';
    *s = 0;
    reverse(start);

    return start;
}

int getop(char *s)
{
    char *p = s;
    int c;
    while((*s = c = getchar()) == ' ' || c == '\t')
        ;

    *++s = 0;
    if(!isdigit(c) && c != '.' && c != '-') return c;

    s = p;
    if(c == '-') 
    {
        int next = getchar();
        if(!isdigit(next) && next != '.')
        {
            ungetc(next, stdin);
            return '-';
        }

        *++s = c = next;
    }

    if(isdigit(c))
        while(isdigit(*++s = c = getchar()))
            ;
    if(c == '.')
        while(isdigit(*++s = c = getchar()))
            ;
    *s = 0;
    if(c != EOF) ungetc(c, stdin);

    return NUMBER;
}

int main(void)
{
    printf("%d\n", strindex("0123456aboba", "aboba"));    
    
    char buf[] = "123456789";
    reverse(buf);
    puts(buf);

    printf("%d\n", atoi("-42"));
    printf("%d\n", atoi("42"));
    int res = atoi("-42123123");
    printf("%d\n", res);
    char num_buf[32];
    itoa(res, num_buf, 10);
    puts(num_buf);    

    char opt_buf[1024];
    int tok = getop(opt_buf);
    printf("%c: %s\n", tok, opt_buf);

    return 0;
}
