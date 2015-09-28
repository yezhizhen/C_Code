#include <stdio.h>
#include <string.h>
//ctrl+d in linux means end of input
int main(){

	char a[20];
	while(scanf("%s",a)){
		if(a[strlen(a)-1]=='.'){
			a[strlen(a)-1]='\0';
			printf("length:%lu \n",strlen(a));
			break;
}
		printf("length: %lu\n ",strlen(a));
		memset(a,'\0',sizeof(a));
}	
	return 0;
}
