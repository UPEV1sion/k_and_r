#include <stdio.h>

#define ALPH_SIZE 256

void print_hor_histo(const unsigned int *word_counts)
{
    for(size_t i = 0; i < ALPH_SIZE; ++i)
    {
        unsigned int cur_len = word_counts[i];
        printf("%2zu:", i);
        while(cur_len > 0){ putchar('='); cur_len--;}
        putchar('\n');
    }
}

void print_ver_histo(const unsigned  int *word_counts)
{
    for(size_t len = ALPH_SIZE; len > 0; --len)
    {
        for(size_t i = 0; i < ALPH_SIZE; ++i)
        {
            if(word_counts[i] <= len - 1) printf("   ");
            else printf("*  ");
        }
        putchar('\n');
    }
    for(size_t i = 0; i < ALPH_SIZE; ++i) printf("%-3zu", i);
    putchar('\n');
}

void stat_input(void)
{

    unsigned int counts[ALPH_SIZE] = {0};

    int c;
    while((c = getchar()) != EOF)
    {
        counts[c]++;
    }

    print_hor_histo(counts);
}


int main (void)
{
    stat_input();

    return 0;
}