#include <stdio.h>
#include <unistd.h>
int main(){
	printf("Current PID: %d\n",getpid());
	fork();
	printf("My PID is %d\n", getpid());

return 0;
}
