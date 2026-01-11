#include <stdio.h>
#include <string.h>

size_t str_len(const char *s)
{
	size_t len = 0;
	while(*s++) len++;
	return len;
}

void str_ncpy(char *dest, const char *src, size_t n)
{
	while(n > 0)
	{
		if(*src)
		{
			*dest++ = *src++;
		}
		else 
		{
			*dest++ = 0;
		}
		n--;
	}
}

void str_ncat(char *dest, const char *src, size_t n)
{
	str_ncpy(dest + str_len(dest), src, n);
}


int str_ncmp(const char *s1, const char *s2, size_t n)
{
	while(n-- > 0) 
	{
		const char c1 = *s1++;
		const char c2 = *s2++;

		if(c1 != c2) return c1 - c2;
		if(c1 == 0) break;
	}


	return 0;
}

int main(void)
{
	char buffer[1024];
	str_ncpy(buffer, "jkljdsklfafjkdlösafjkalsdfkj", 10);
	buffer[10] = 0;
	puts(buffer);

	str_ncat(buffer, " hello", 20);
	buffer[20] = 0;
	puts(buffer);

	printf("%d\n", strncmp("das ist ein test", "das ist ein test", 10));
	printf("%d\n", str_ncmp("das ist ein test", "das ist ein test", 10));

	return 0;
}
