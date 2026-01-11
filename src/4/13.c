#include <stdio.h>
#include <string.h>

void rec_reverse(char *s, const size_t i, const size_t len)
{
    const size_t j = len - 1 - i;
    if(j < i) return;

    const char c = s[i];
    s[i] = s[j];
    s[j] = c;
    
    rec_reverse(s, i + 1, len);
}

void reverse(char *s)
{
    rec_reverse(s, 0, strlen(s));
}

int main(void)
{

    char buffer[] = "katze";
    puts(buffer);
    reverse(buffer);
    puts(buffer);

    return 0;
}
  
