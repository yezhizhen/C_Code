#include <stdio.h>

int main(){
	int a[3][2]={{0,1},{2,3},{3,4}};	
	int *arrp[3]=&a[0][0];
	
	printf("%d",arrp[0][0]);
	printf("%d",arrp[1][1]);

}






