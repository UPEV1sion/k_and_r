#include <stdio.h>

char* str_cat(char *dest, const char *src)
{
	char *start = dest;
	while(*dest) dest++;

	while(*src) *dest++ = *src++;

	*dest = 0;

	return start;
}

int main(void)
{
	char buf[1024] = "hello ";
	str_cat(buf, "world\n");

	puts(buf);

	return 0;
}
