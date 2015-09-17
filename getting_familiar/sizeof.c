#include <stdio.h>
#include <stdlib.h>

int main(){
	int a[5]={0,1,2};
	int *pa;
	//print long unsigned int
	printf("sizeof pa before assigning:%lu\n",sizeof(pa));
	pa = a;
	printf("sizeof pa after assigning: %lu\n",sizeof(pa));
	pa = malloc (sizeof(*pa)*10);
	printf("sizeof pa after malloc: %lu\n",sizeof(pa));
	printf("sizeof a[0]:%lu\n",sizeof(a[0]));
	printf("sizeof array a: %lu\n", sizeof(a));
	int t=3;
	int pt=(int)&t;
	printf("0x%x\n",&t);
	printf("0x%x\n",pt);
	printf("%p\n",&t);
	return 0;
}
