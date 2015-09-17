#include <stdio.h>
#include <stdlib.h>

int main(){
	int a[] = {5, 15, 34, 54, 14, 2, 52, 72};
	int *p = &a[5];
	printf("%d",p[-2]);
	return 0;
}
