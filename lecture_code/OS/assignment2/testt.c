#include <stdio.h>


int main()
{
	unsigned int num = 0xF0000001;
	num -= 0xF0000000;
	printf("%u\n",num);

}
