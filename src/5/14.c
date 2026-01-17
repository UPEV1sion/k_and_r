#include <stdio.h>
#include <string.h>

#define MAXLINE 5000
char *lineptr[MAXLINE];

int readlines(char *lineptr[], int nlines);
int writelines(char *lineptr[], int nlines);

void quick_sort(void *lineptr, int left, int right, int (*comp) (void *, void *));
int numcmp(char *, char *)

int main(int argc, char **argv)
{
	int nlines;
	int numeric = 0;

	if(argc > 1 && strcmp(argv[1], "-n") == 0)
	{
			numeric = 1;
	}                
	if((nlines = readlines(lineptr, MAXLINE)) >= 0)
	{
		qsort(lineptr, 0, nlines - 1, numeric ? numcpy : strcmp);
		writelines(lineptr, nlines);
		return 0;
    }
    else 
    {
        printf("input too big to sort\n");
        return 1;
    }
 
 
	return 0;
}
