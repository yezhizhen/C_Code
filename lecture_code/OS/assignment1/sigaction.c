#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

void sighandler(int);

int main()
{
	char* oper=malloc(sizeof(*oper)*11);
	struct sigaction oldact;
	struct sigaction newact;
	newact.sa_handler = sighandler;
	sigset_t nm = newact.sa_mask;
	sigemptyset(&nm);
	if(sigaction(SIGINT,NULL,&oldact)==-1)
	{
		perror("error...\n");
	}	
	else
	{
		if(oldact.sa_handler==SIG_DFL)
		{
			printf("original SIGINT is signal  default mechanism\n");
		}
		else if(oldact.sa_handler==SIG_IGN)
		{

			printf("originally SIGINT signal SIGINT is ignored\n");
		}
	}

	/*if(sigaction(SIGINT,&newact,NULL)==-1)
	{
		perror("error to set new sigaction\n");
	}*/
	if(sigaction(SIGQUIT,&newact,NULL)==-1)
	{
		perror("error to set new sigaction\n");
	}
	while(1)
	{
		//block the signal
		if(strcmp(oper,"block")==0)
		{
			sigaddset(&nm,SIGINT);
			sigprocmask(SIG_BLOCK,&nm,NULL);
			printf("SIGINT BLOCKED\n");				
		}
		//unblock the signal
		else if(strcmp(oper,"unblock")==0)
		{
			sigprocmask(SIG_UNBLOCK,&nm,NULL);
			printf("SIGINT UNBLOCKED\n");
		}
		sleep(1);
		printf("sleeping....\n");
		scanf("%10s",oper);
		


	}
	

}

void sighandler(int signum)
{
	printf("Caught signal %d, coming out...\n", signum);
}
