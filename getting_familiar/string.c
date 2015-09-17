#include <stdio.h>
#include <string.h>
#include <stdlib.h>
int main(){
	char a[8];
	char b[8];
	//char *c=malloc(sizeof(*c)*100);
	//insecure scanf:   scanf("%s", a);
	//secure scanf
	//wrong way: char *ps; scanf("%s", ps);
	
	scanf("%7s",a);
	scanf("%7s",b);
	//scanf("%s",c);
	
	printf("%s##%s##\n",a,b);
	//printf("address of c: %p\n",c);
	//printf("content of c: %s\n", c);
	return 0;
}
