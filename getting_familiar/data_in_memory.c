#include <stdio.h>

int main(){
	char a = 254;
	int b = 254;
	printf("char a=%d, int b= %d\n",a,b);
	unsigned char c=254;
	char d= 254u;
	printf("unsigned char c:%d,char d with suffix u:%d\n",c,d);
	char k= 127;
	k++;
	printf("char 127+1=%d\n",k);
	return 0;
}
