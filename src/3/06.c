#include <stdio.h>
#include <string.h>

#define abs(a) ((a) < 0 ? -(a) : (a))

void reverse(char *s)
{

    size_t len = strlen(s);

    for(size_t i = 0; i < len/2; ++i)
    {
        const char temp = s[i];
        s[i] = s[len-1-i];
        s[len-1-i] = temp;
    }
}

void itoa(int num, char *s, const int min_len)
{

    int sign = num;
    int i = 0;

    do
    {
        s[i++] = abs(num % 10) + '0';
    } while(num /= 10);

    if(sign < 0) s[i++] = '-';

    while(i < min_len)
    {
        s[i++] = '0';
    }

    s[i] = 0;
    
    reverse(s);
}

int main(void)
{
    char buffer[1024];
    itoa(69, buffer, 10);

    puts(buffer);

    return 0;
}

