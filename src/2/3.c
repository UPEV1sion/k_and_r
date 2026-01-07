#include <stdio.h>
#include <ctype.h>

int htoi(const char *s)
{
    if(s == NULL || *s == 0) return -1;
    if(s[0] == '0' && tolower(s[1]) == 'x') s += 2;
    
    int res = 0;
    while(*s)
    {
        int val;
        if(*s >= '0' && *s <= '9')
        {
            val = *s - '0';
        } 
        else
        {
            int lower = tolower(*s);
            if(lower >= 'a' && lower <= 'f') val = lower - 'a' + 10;
            else return -1;
        }

        res = res * 16 + val;

        s++; 
    }

    return res;
}

int main(void)
{

    printf("%d\n", htoi("0x1ff"));

    return 0;
}
