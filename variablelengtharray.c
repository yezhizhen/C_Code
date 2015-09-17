#include <stdio.h>
#include <string.h>
//#define t 3
int main(){
	const int a=3;
	int b[a];
	memset(b,2,sizeof(b));
	//you fill every byte with 0x02
	int *pb = b;
	for(int k=0;k<a;k++){
		printf("%d, ", pb[k]);
}
	printf("\n");
	return 0;
}
