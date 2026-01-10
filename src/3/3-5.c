#include <stdio.h>
#include <string.h>

#define abs(a) ((a) < 0 ? -(a) : (a))

void reverse(char *s)
{
	const size_t len = strlen(s);

	for(size_t i = 0; i < len/2; ++i)
	{
		const char temp = s[i];
		s[i] = s[len-1-i];
		s[len-1-i] = temp;
	}
}

void itob(int num, char *s, const int base)
{
	static const char conv[] = "0123456789abcdefghijklmnopqrstuvwxyz";

	int sign = num;
	int i = 0;	

	do
	{
		s[i++] = conv[abs(num % base)];
	} while(num /= base);

	if(sign < 0) s[i++] = '-';

	s[i] = 0;

	reverse(s);
}

int main(void)
{
	char buffer[1024];
	itob(69, buffer, 16);

	puts(buffer);

	return 0;
}

	
