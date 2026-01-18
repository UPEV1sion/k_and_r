#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

#define MAXLINE 5000
char *lineptr[MAXLINE];

int readlines(char *lines[], int nlines);
void writelines(char *lines[], int nlines, bool reverse);

void quick_sort(void *lineptr[], int left, int right, int (*comp) (void *, void *));
int num_cmp(void *, void *);
int str_cmp(void *, void *);
int str_case_cmp(void *, void *);

enum Flags {
    NUMERIC = (1 << 0),
    REVERSE = (1 << 1),
};

static unsigned char options;

void get_flags(int argc, char **argv)
{
    while(--argc > 0 && (*++argv)[0] == '-')
    {
        char *cur_flag = *argv;
        while(*++cur_flag)
        {
            switch(*cur_flag)
            {
                case 'r': options |= REVERSE; break;
                case 'n': options |= NUMERIC; break;
                default: 
                    fprintf(stderr, "Unknown option -%c\n", *cur_flag); 
                    exit(1);
                    break;
            }
        }
    }
}

int main(int argc, char **argv)
{
	int nlines;
    get_flags(argc, argv);

	if((nlines = readlines(lineptr, MAXLINE)) >= 0)
	{
		quick_sort((void **)lineptr, 0, nlines - 1, (options & NUMERIC) ? num_cmp: str_cmp);

		writelines(lineptr, nlines, (options & REVERSE) != 0);
		return 0;
    }

    printf("input too big to sort\n");
    return 1;
}

#define ALLOCSIZE 10000
static char allocbuf[ALLOCSIZE];
static char *allocp = allocbuf;

char* alloc(const int n)
{
    if(allocbuf + ALLOCSIZE - allocp >= n)
    {
        allocp += n;
        return allocp - n;
    }

    return 0;
}

void afree(char *p)
{
    if(p >= allocbuf && p < allocbuf + ALLOCSIZE)
        allocp = p;
}

void areset(void)
{
    allocp = allocbuf;
}

int get_line(char *buf, int bufsize)
{
    const char *start = buf;
    int c;
    while(--bufsize > 0 && (c = getchar()) != EOF)
    {
        *buf++ = c;
        if(c == '\n') break;
    }

    *buf = 0;

    return (int) (buf - start);
}

int readlines(char *lines[], const int maxlines)
{
    int nlines = 0;

    char line[MAXLINE];

    int len;
    while((len = get_line(line, MAXLINE)) > 0)
    {
        char *p;
        if(nlines >= maxlines || (p = alloc(len)) == NULL)
        {
           return -1;
        }
        else
        {
            line[len - 1] = 0;
            strcpy(p, line);
            lines[nlines++] = p;
        }
    }

    return nlines;
}

void writelines(char *lines[], const int nlines, const bool reverse)
{
    for(int i = 0; i < nlines; ++i)
    {
        puts(reverse ? lines[nlines - i - 1] : lines[i]);
    }
}

int num_cmp(void *s1, void *s2)
{
    const double v1 = atof((char *)s1);
    const double v2 = atof((char *)s2);
    return (v1 > v2) - (v1 < v2);
}

int is_dir_char(int c)
{
    return isalnum(c) || c == ' ';
}

int str_cmp(void *v1, void *v2)
{
	char *s1 = v1;
	char *s2 = v2;
	
	for(;;)
	{
		char c1 = *s1;
		char c2 = *s2;
		
		if(c1 != c2) return (c1 > c2) - (c1 < c2);
		if(c1 == 0) return 0;
		
		s1++;
		s2++;
	}
}

 void swap(void *v[], const int i , const int j)
{
    void *temp = v[i];
    v[i] = v[j];
    v[j] = temp;
}

void quick_sort(void *v[], const int left, const int right, int (*comp) (void *, void *))
{
    if(left >= right) return;

    swap(v, left, (left + right) / 2);
    int last = left;
    for(int i = left + 1; i <= right; ++i)
    {
        if(comp(v[i], v[left]) < 0)
        {
            swap(v, ++last, i);
        }
    }   

    swap(v, left, last);    
    quick_sort(v, left, last - 1, comp);
    quick_sort(v, last + 1, right, comp);
}
