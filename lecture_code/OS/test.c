#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
int main(){
	int *status;
	wait(status);	
//	printf("%d",*status);
	return 0;
}
