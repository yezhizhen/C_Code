#include <stdio.h>


void print(int a[]);
int main(){
	int a[4]={0,1,2,3};
	print(a);	
	return 0;
}

void print(int a[]){
	printf("%d",a[0]);
}
