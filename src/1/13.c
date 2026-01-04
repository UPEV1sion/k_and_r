#include <stdio.h>
#include <stddef.h>
#include <assert.h>

#define MAX_LEN 20

enum State
{
    IN, OUT
};

void print_hor_histo(const unsigned int *word_lens)
{
    for(size_t i = 0; i < MAX_LEN; ++i)
    {
        unsigned int cur_len = word_lens[i];
        printf("%2zu:", i);
        while(cur_len > 0){ putchar('='); cur_len--;}
        putchar('\n');
    }
}

void print_ver_histo(const unsigned  int *word_lens)
{
    for(size_t len = MAX_LEN; len > 0; --len)
    {
        for(size_t i = 0; i < MAX_LEN; ++i)
        {
            if(word_lens[i] <= len - 1) printf("   ");
            else printf("*  ");
        }
        putchar('\n');
    }
    for(size_t i = 0; i < MAX_LEN; ++i) printf("%-3zu", i);
    putchar('\n');
}

void stat_input(void)
{
    int wl = 0;
    unsigned int word_lens[MAX_LEN] = {0};
    enum State state = OUT;

    int c;
    while ((c = getchar()) != EOF)
    {
        assert(wl < MAX_LEN && "One input word is too long");
        if (c == ' ' || c == '\t' || c == '\n')
        {
            if(state == IN)
                word_lens[wl]++;
            wl = 0;
            state = OUT;
        }
        else
        {
            wl++;
            state = IN;
        }
    }

    print_hor_histo(word_lens);
    print_ver_histo(word_lens);
}


int main(void)
{
    stat_input();

    return 0;
}