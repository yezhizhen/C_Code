#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>


void see(int ar[]);

int main()
{
	int a = 3;
	int b[a];	
	memset(b,0,a);
	printf("b[0]:%d\n",b[0]);
	printf("main sizeof(b):%lu\n",sizeof(b));
	see(b);
	return 0;
}

void see(int ar[])
{
	printf("see sizeof(b):%lu\n",sizeof(ar));
	

}
