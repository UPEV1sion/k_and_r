#include <stdio.h>

int check_escape_sequences(const char cur, const char prev, const char quote)
{
    static const char allowed_escapes[] = "abtnvfr\\";

    if(prev == '\\')
    {
        for(size_t i = 0; i < sizeof allowed_escapes - 1; ++i)
        {
            if(cur == allowed_escapes[i] || cur == quote)
            {
                return 0;
            }
        }

        fprintf(stderr, "Invalid escape sequence found: \\%c\n", cur);
        return 1;
    }

    return 0;
}

int check_string(const char quote)
{
    int prev = quote;
    int cur;
    while((cur = getchar()) != EOF)
    {
        if(cur == quote && prev != '\\') return 0;

        if(check_escape_sequences(cur, prev, quote) != 0) return 1;

        if(cur == '\n') 
        {
            fprintf(stderr, "Unterminated string at line end\n");
            return 1;
        }

        prev = cur;
    }

    if(cur == EOF) 
    {
        fprintf(stderr, "Unterminated string at file end\n");
        return 1;
    }

    return 0;
}

void count_parens(const char c, int *brace_count, int *paren_count, int *bracket_count)
{
    switch (c)
    {
        case '{': (*brace_count)++; break;
        case '}': (*brace_count)--; break;
        case '(': (*paren_count)++; break;
        case ')': (*paren_count)--; break;
        case '[': (*bracket_count)++; break;
        case ']': (*bracket_count)--; break;
        default: break;
    }
}

int check_comment(void)
{
    int cur = getchar();

    if(cur == '/')
    {
        while((cur = getchar()) != EOF && cur != '\n')
            ;
        return 0;
    }

    if(cur == '*')
    {
        int prev = 0;
        int depth = 1;
        while((cur = getchar()) != EOF)
        {
            if (prev == '/' && cur == '*') depth++;
            if (prev == '*' && cur == '/') depth--;
            if (depth > 1)
            {
                fprintf(stderr, "Nested comment detected\n");
                return 1;
            }

            if (depth == 0) return 0;

            prev = cur;
        }

        fprintf(stderr, "Unterminated block comment\n");
        return 1;
    }

    return 0;
}

void check_syntax(void)
{

    int brace_count = 0, paren_count = 0, bracket_count = 0;

    int c;
    while ((c = getchar()) != EOF)
    {
        if (c == '/')
        {
            const int next = getchar();
            if (next == '/' || next == '*')
            {
                ungetc(next, stdin);
                if (check_comment() != 0) return;
            }
            else
            {
                ungetc(next, stdin);
            }
        }

        if(c == '"' || c == '\'')
        {
            if (check_string(c) != 0) return;
            continue;
        }

        count_parens(c, &brace_count, &paren_count, &bracket_count);
    }

    if (brace_count) fprintf(stderr, "Braces not balanced\n");
    else if (paren_count) fprintf(stderr, "Parens not balanced\n");
    else if (bracket_count) fprintf(stderr, "Brackets not balanced\n");
    else puts("Everything ok in this file");
}

int main()
{
    check_syntax();
    return 0;
}
