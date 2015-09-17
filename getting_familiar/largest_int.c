#include <stdio.h>

int main(){
	int a =0;
	while(++a>0);
	printf("largest unsigned int:%d\n",--a);
	int digit;
	while(a){
		digit++;
		a=a/10;
}
	printf("number digit of it: %d",digit);
	return 0;
}
