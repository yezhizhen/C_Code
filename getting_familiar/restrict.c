#include <stdio.h>
#include <string.h>

//according to manul, strings may not overlap, and the destination string dest must be large enough to receive the copy.to
	//because it is restricted
	//if overlap, function has undefined behavior
int main()
{	
	char a[]="abacacs";
	char* pa=a;
	printf("pa:%s\n",pa);

	strcpy(pa,pa);
	printf("pa:%s\n",pa);
	return 0;
}
