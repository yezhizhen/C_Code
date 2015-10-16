#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main()
{
	int pipefd[2];
	pipe(pipefd);
	char* buf="fbcd";
	char* redbuf= malloc(sizeof(char)*5);
	printf("pipefd[0]:%d\n", pipefd[0]);

	printf("pipefd[1]:%d\n", pipefd[1]);
	char *a[3] = {"cat","test.txt",NULL};
	char *b[2] = {"cat",NULL};
	printf("use without fork\n");
	write(pipefd[1], buf,5);
	read(pipefd[0],redbuf,4);
    fprintf(stdout,"read from itself:%s\n",redbuf);	
	int childpid;
	//let child write
	if((childpid=fork())==0)
	{
		printf("child: pipefd[0]:%d\n",pipefd[0]);
		printf("child: pipefd[1]:%d\n",pipefd[1]);
		close(pipefd[0]);
		dup2(pipefd[1],STDOUT_FILENO);
		execvp(a[0],a);
	}	
	else
	{
		close(pipefd[1]);
		waitpid(childpid,NULL,WUNTRACED);
		//if comment this part, file cat would read from stdin
		//dup2(pipefd[0],STDIN_FILENO);
		execvp(b[0],b);
	}
	
	return 0;
}
