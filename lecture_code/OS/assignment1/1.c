#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main()
{
	char** a=malloc(sizeof(*a)*50);
	for(int i=0;i<50;i++)
	{
		a[i]=malloc(sizeof(**a)*10);
	}
	strcpy(a[3],"abcd");
	printf("a:%s\n",a[3]);
	a[3][0]='b';
	printf("a:%s\n",a[3]);
//	char* c=malloc(sizeof(*c)*10);
	char c[]="abcd";
	printf("c:%s\n",c);
	c[0]='b';
	printf("c:%s\n",c);
//But you cannot modify constant string
	char* d = malloc(sizeof(*d)*10);
//in this way, the string is constant
	d="abcd";

	printf("d:%s\n",d);
	d[0]='b';
	printf("d:%s\n",d);
	return 0;
}
