#include <unistd.h>
#include <stdio.h>

int main()
{
	int ival = 0x80;
	char *p;
	p = (char *) &ival;
	for (int i = 0; i < 4; i++, p++)
	{
		printf("%02x ", *p);
	}
	return 0;
}