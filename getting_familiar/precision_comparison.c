#include <stdio.h>
#define size 8
#define a1 2.2
#define a2 1.1
#define b1 1.2
#define b2 0.1

int main()
{
	double a = a1-a2;
	double b = b1-b2;
	char* first = (char*)&a;
	char* second = (char*)&b;
	printf("%f-%f==%f-%f? :%d\n",a1,a2,b1,b2,a==b);
	printf("first memory allocation:\n");
	for(int i=0;i<size;i++)
	{
		printf("%02hhX",*first);
		first++;
	}
	putchar('\n');
	printf("second memory allocation:\n");
	for(int i=0;i<size;i++)
	{
		printf("%02hhX",*second);
		second++;
	}
	putchar('\n');
	//2.5-1.1 == 1.5-0.1. True
	printf("1.5-0.1==1.5-0.1?%d\n",(2.5-0.1)==(1.5-0.1)); //False
	//printf("2.2-1.1==1.2-0.1? :%d\n",(2.2-1.1)==(1.2-0.1));
	
	return 0;
}
