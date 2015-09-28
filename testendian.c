#include <stdio.h>
#define INT_BYTES 4

int main()
{
	unsigned int a = 0xfabc12bf;
	printf("unsigned int:%x\n",a);
	unsigned char *p=(char*)&a;
	printf("address:%p\n",p);
	for(int k=0;k<INT_BYTES;k++)
	{
		printf("value:%x\n",*p);
		p++;
	}
	return 0;
}
