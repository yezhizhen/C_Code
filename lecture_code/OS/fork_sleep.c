#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <sys/types.h>
int main(int argc, char** argv){
	for(int k=0;k<argc;k++){
		printf("%s\n",*(argv++));
}
	printf("Before fork\n");
	int result;
	result = fork();
	//child process
	if(result == 0){
		printf("I'm child\n");
		sleep(1);
		printf("I, child, has  PID: %d\n",getpid());		
		printf("child terminated\n");
}	
	//parent process
	else{
		printf("I'm parent\n");
		printf("I, parent, has  PID: %d\n",getpid());	
		sleep(1);
		printf("parent terminated\n");
		wait(NULL);
}

	return 0;
}
