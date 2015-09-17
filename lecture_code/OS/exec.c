#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
int main(){
	printf("before execl,parent:%d\n",getpid());
	
	if(!fork()){
		printf("I'm child: %d\n",getpid());
		sleep(1);
	//	execl("/bin/ls","/bin/ls","-l",NULL);
		execl("./fork_sleep","./fork_sleep",NULL);
	}
	wait(NULL);	
	printf("After execl\n");

	return 0;
}
