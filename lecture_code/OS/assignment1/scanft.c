#include <stdio.h>

int main()
{
	char *p=malloc(sizeof(*p)*10);
	scanf("%s",p);
	printf("%s",p);
	return 0;
}
