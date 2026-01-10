#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>

bool is_valid_range(const char start, const char stop)
{
    return (isdigit(start) && isdigit(stop)) || (isalpha(start) && isalpha(stop));
}

void expand(const char *s1, char *s2)
{
    size_t read = 0;
    size_t write = 0;
    while(s1[read])
    {
        if(s1[read] == '-' && read > 0 && s1[read+1] != 0 && is_valid_range(s1[read-1], s1[read+1]))
        {
            char start = s1[read-1] + 1;
            char stop = s1[read+1];
            while(start <= stop)
            {
                s2[write++] = start++;
            }

            read += 2;
        }
        else
        {
            s2[write++] = s1[read++];
        } 
    }
        
    s2[write] = 0;
}


int main(void)
{
    char buffer[1024];
    expand("-a-z0-9-", buffer);
    puts(buffer);

    return 0;
}
