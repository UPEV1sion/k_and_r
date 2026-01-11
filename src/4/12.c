#include <stdio.h>

void rec_itoa(const int val, char *str, int *i)
{
    if (val == 0) return;

    rec_itoa(val / 10, str, i);
    str[(*i)++] = (char) (val % 10 + '0');
}

char* itoa(int val, char *str)
{
    int i = 0;

    if(val < 0)
    {
        str[i++] = '-';
        val = -val;
    }

    rec_itoa(val, str, &i);

    str[i] = 0;
    return str;
}

int main(void)
{

    char buffer[1024];
    printf("%s\n", itoa(-42, buffer));

    return 0;
}
