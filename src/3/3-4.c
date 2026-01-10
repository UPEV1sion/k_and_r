#include <stdio.h>
#include <limits.h>

#define abs(a) ((a) < 0 ? -(a) : (a))

size_t str_len(const char s[])
{
	size_t len = 0;
	while(*s++) len++;
	return len;
}

void reverse(char s[])
{
	size_t right = str_len(s) - 1;
	size_t left = 0;

	while (left < right)
	{
		const char temp = s[right];
		s[right--] = s[left];
		s[left++] = temp;
	}
}

void itoa(int num, char s[]) 
{
	int i = 0;
	int sign = num;

	do
	{
		s[i++] = abs(num % 10) + '0';
	} while(num /= 10);
   
	if (sign < 0) s[i++] = '-';

	s[i] = 0;
	reverse(s);
}

int main(void)
{
	char buffer[1024];
	itoa(INT_MIN, buffer);

	puts(buffer);	


	return 0;
}
