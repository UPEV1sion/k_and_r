#include <stdio.h>
#include <string.h>

int strend(const char *t, const char *s)
{
	const size_t len_t = strlen(t);
	const size_t len_s = strlen(s);

	if(len_s > len_t) return 0;

	for(size_t i = 0; i < len_s; ++i)
	{
		if(t[len_t - len_s + i] != s[i]) return 0;
	}


	return 1;
}


int main(void)
{
	puts(strend("foo baz bar aboba", "aboba") ? "true" : "false");

	return 0;
}

