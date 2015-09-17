#include <stdio.h>
#include <string.h>
#include <stdlib.h>
int main(){
	char a[5];
	char b[4];
	//char *c=malloc(sizeof(*c)*100);
	//insecure scanf:   scanf("%s", a);
	//secure scanf
	//wrong way: char *ps; scanf("%s", ps);
	printf("sizeof(char):%u\n",sizeof(char));	
	scanf("%s",a);
	scanf("%s",b);
	//scanf("%s",c);
	printf("address of a[0]:%p\n",a);
	printf("address of b[0]:%p\n",b);
	printf("%s##%s##\n",a,b);
	//printf("address of c: %p\n",c);
	//printf("content of c: %s\n", c);
	return 0;
}
