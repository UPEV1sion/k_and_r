#include <stdio.h>

// Single line comments are not part of ansi-c but are also handled in here

/*
 * This multiline comment will get removed
 */

void skip_comment(void)
{
    int prev = getchar();
    int cur = getchar();
    while (prev != '*' || cur != '/')
    {
        prev = cur;
        cur = getchar();
    }
}

void print_literal(const int start_escape)
{
    int c;
    putchar(start_escape);
    while ((c = getchar()) != start_escape)
    {
        putchar(c);
        if (c == '\\') putchar(getchar());
    }
    putchar(c);
}

void remove_comments(void)
{
    int cur, next;
    while((cur = getchar()) != EOF)
    {
        if(cur == '/')
        {
            if((next = getchar()) == '*')
            {
                skip_comment();
            }
            else if(next == '/')
            {
                while((cur = getchar()) != EOF && cur != '\n')
                    ;
            }
            else
            {
                putchar(cur);
                putchar(next);
            }
        }
        else if (cur == '\'' || cur == '"')
        {
            print_literal(cur);
        }
        else
        {
            putchar(cur);
        }
    }
}

int main()
{
    remove_comments();
    return 0;
}
