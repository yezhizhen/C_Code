#include <stdio.h>
#include <string.h>
int main(){
	char a[20];
	while(scanf("%s",a)){
		if(a[strlen(a)-1]=='.'){
			a[strlen(a)-1]='\0';
			printf("%lu ",strlen(a));
			break;
}
		printf("%lu ",strlen(a));
		memset(a,0,sizeof(a));
}	
	return 0;
}
