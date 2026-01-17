#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#define MAXLINE 1024
#define TABSIZE 4

#define shift_args(argc, argv) ((argc)--, *(argv)++)

int next_tab(int pos, const bool *tab_pos)
{
	while(pos + 1 <= MAXLINE)
	{
		if(tab_pos[pos + 1]) return pos + 1;
		pos++;
	}

	return MAXLINE + 1;
}

void entab(const bool *tab_pos)
{
	int pos = 0;
	int space_count = 0;
	
	int c;
	while((c = getchar()) != EOF)
	{
		if(c == ' ')
		{	
			space_count++;
			if(pos + 1 <= MAXLINE && tab_pos[pos + 1])
			{
				putchar('\t');
				space_count = 0;
			}
		}
		else
		{
			for(; space_count > 0; space_count--) putchar(' ');
			putchar(c);
			
			if(c == '\n') pos = -1;
			else if(c == '\t') pos = next_tab(pos, tab_pos) - 1;	
		}

		pos++;
	}
}

void detab(const bool *tab_pos)
{
	int pos = 0;
	
	int c;
	while((c = getchar()) != EOF)
	{
		if(c == '\t')
		{
			do
			{
				putchar(' ');
				pos++;
			} while(pos <= MAXLINE && !tab_pos[pos]);
		}
		else
		{
			putchar(c);
			pos++;
			if(c == '\n') pos = 0;
		}
	}
}

void settab(int argc, char **argv, bool *tab_pos)
{
    if(argc <= 1) 
    {
        for(int pos = 1; pos <= MAXLINE; pos += TABSIZE) tab_pos[pos] = true; 
    }
    else if(argc == 3 && *argv[1] == '-' && *argv[2] == '+')
    {
		shift_args(argc, argv);
		char *start_str = shift_args(argc, argv);
		char *repeat_str = shift_args(argc, argv);
		
		int start = atoi(++start_str);
		int repeat = atoi(++repeat_str);
		for(; start <= MAXLINE; start += repeat) tab_pos[start] = true;
    }
    else
    {
        while(--argc > 0)
        {
            int pos = atoi(*++argv);
            if(pos < MAXLINE && pos > 0) tab_pos[pos] = true;
            else fprintf(stderr, "error: tab position too large!\n"); 
        }
    }
}

int main(int argc, char **argv)
{
	while(argc) puts(shift_args(argc, argv));	

	return 0;	

	bool tab_pos[MAXLINE + 1] = {0};
	settab(argc, argv, tab_pos);
	
	puts("entab:");
	entab(tab_pos);
	puts("detab:");
	detab(tab_pos);
	return 0;
}

