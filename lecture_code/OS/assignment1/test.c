#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
int main()
{
	pid_t childpid;
	char *s[2]={"cat",NULL};
	struct sigaction shell;
	sigemptyset(&shell.sa_mask);
	shell.sa_handler=SIG_IGN;
	sigaction(SIGTSTP,&shell,NULL);
	if((childpid=fork())==0)
	{
		execvp(s[0],s);
		exit(EXIT_SUCCESS);
	}
	//signal(SIGTSTP,SIG_IGN);
	int status;
	waitpid(childpid,&status,WUNTRACED);
	if(WIFSTOPPED(status))
	{
		printf("cat is sleeping!\n");
		printf("wake it up?\n");
		while(getchar()!='Y');
		printf("wake up the child!\n");
		kill(childpid,SIGCONT);
		waitpid(childpid,&status,WUNTRACED);
		
	}

}
