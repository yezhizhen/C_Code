#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main()
{
	//if you use 2d array, you must give specific size for second slot
	char a[][10]={"abcd",
					"dfea"};
	char* b=malloc(sizeof(*b)*10);
	strncpy(b,"abcde",10);
	printf("try modify strcpyed char*..\n");
	b[0]='c';
	printf("finish\n");
	
	char c[10]="abcde";
	printf("try modify array c\n");
	c[0]='b';
	printf("finished\n");

	char *d="abcde";
	printf("try modify c string d\n");
	d[0]='b';

	return 0;
}
