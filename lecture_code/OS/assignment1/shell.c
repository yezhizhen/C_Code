#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#define TRUE 1
#define STRING_EQUAL 0
#define MAXIMUM_SIZE 255
#define MAXIMUM_P_SIZE 257
#define NOT_FOUND(in) printf("%s: command not found\n",in)

int splitLine(char* str,char** tokens);
void cdfunction(char** tokens, int length);
void initialize(char** tokens);
void executeFile(char *result_path,const char* const path,char** tokens,char* in);
//int parent_pid;


void shellsighandler(int signum)
{
	if(SIGINT==signum||SIGTERM==signum||SIGQUIT==signum||SIGTSTP==signum)
	{
		printf("\n");
	}
}


int main()
{	
	//255 characters allowed
	int st;
	int i; 
	char buffer;
	int length;
	int continueflag=0;
//	int position=0;
	char *in = malloc (sizeof(*in)*(MAXIMUM_P_SIZE));
	char* result_path = malloc(sizeof(*result_path)*260);
//	char** tokens = malloc(sizeof(*tokens)*MAXIMUM_SIZE);
	char *tokens[255];
//	char* abcd=malloc(sizeof(char)*30);
//	memset(abcd,0,30);
	//
	char* cwd = malloc(sizeof(*cwd)*200);	
	struct sigaction shell;
//	sigset_t childset = child.sa_mask;
	sigset_t shellset = shell.sa_mask;
//	child.sa_handler = SIG_DFL;
	sigemptyset(&shellset);
	//sigemptyset(&childset);
//	shell.sa_handler = SIG_IGN;	
	shell.sa_handler = shellsighandler;	
//	sigaction(SIGINT,&shell,NULL);
	sigaction(SIGTERM,&shell,NULL);
	sigaction(SIGQUIT,&shell,NULL);
	sigaction(SIGTSTP,&shell,NULL);
//	parent_pid = getpid();
	while(TRUE)
	{
//		position = 0;
		//set all strings to null
		initialize(tokens);
		memset(in,0,MAXIMUM_P_SIZE);
		//basic prestring: path and shell name
		printf("[My Shell:");
		printf("%s",getcwd(cwd,200));
		printf("]$ ");
		//reading input and check if exceeds limit
		//if(scanf("%256s", in) == EOF) continue;
		/*while((buffer=getchar())!='\n'&&buffer!=EOF)
		{	
			if(position==255)
			{
			printf("Exceed maximum length 255\n");
			while((buffer=getchar())!='\n'&& buffer != EOF);
			continueflag = TRUE;
			break;
			}
			in[position++]=buffer;
		}*/
		fgets(in,MAXIMUM_P_SIZE,stdin);
		if(in[strlen(in)-1]!='\n')
		{
			printf("Exceed maximum length 255\n");
			while((buffer=getchar())!='\n'&& buffer != EOF);
			continueflag = TRUE;
		}
		in[strlen(in)-1]='\0';
		//if empty string 
		if(!strlen(in)) continue; 
		if(continueflag)	continue;
		
		//try to get all parameters
		length = splitLine(in,tokens);			
		//switch case		
		if(strcmp(tokens[0],"help") == STRING_EQUAL)
		{
			printf("List of functions you need\n");
		}
		//else if(*in=='') continue;
		else if(tokens[0][0]=='/'||tokens[0][0]=='.')
		{
			//create and run child in if
			if(!fork())
			{
				shell.sa_handler = SIG_DFL;
				if(execv(tokens[0],tokens) == -1) 
				{
				//save the errno
				int errsv = errno;
				//printf("error number is%d\n",errsv);								
				if(errsv==ENOENT)	NOT_FOUND(in);
				else printf("%s: unknown error",in);	
				}
				exit(EXIT_SUCCESS);
			}
			//parent wait for the child
		}
		//deal with cd comand
		else if(strcmp(tokens[0],"cd")==STRING_EQUAL)
		{
			cdfunction(tokens,length);
		}
		else if(strcmp(tokens[0],"exit")==STRING_EQUAL)
		{
			if(length==1)
			{
				exit(EXIT_SUCCESS);
			}
			else	printf("exit: wrong number of arguments\n");
		}
		//deal with file name input
		else
		{
			//child process
			if(fork()==0)
			{
				shell.sa_handler = SIG_DFL;
				executeFile(result_path,"/bin/",tokens,in);
				executeFile(result_path,"/usr/bin/",tokens,in);
				executeFile(result_path,"./",tokens,in);
				exit(EXIT_FAILURE);
			}
		}
		wait(NULL);
	}
	free(result_path);
	free(tokens);
	free(in);	
	free(cwd);
	return 0;
}

int splitLine(char *str, char** tokens)
{
	int k=0;
	tokens[0] = strtok(str," \t");    		
	while(tokens[k]!=NULL)
	{
   		k++;
    	tokens[k] = strtok(NULL," ");
	}
	return k;
}

void cdfunction(char** tokens, int length)
{
	if(length!=2) printf("cd: wrong number of arguments\n");
	else if(chdir(tokens[1])==-1) printf("[%s]: cannot change directory\n",tokens[1]);
}

void initialize(char** tokens)
{
	int i;
	for(i=0;i<MAXIMUM_SIZE;i++)
	{
		tokens[i]="";
	}	
}

void executeFile(char *result_path,const char* const path,char** tokens,char* in)
{
	strcpy(result_path,path);
	strcat(result_path, tokens[0]);
	tokens[0] = result_path;
	if(execv(result_path,tokens) == -1) 
	{
		//save the errno
		int errsv = errno;
	//	printf("errno:%d\n",errsv);
		if(errsv!=ENOENT)	printf("%s: unknown error\n",in);	
		else if(strcmp(path,"./")==STRING_EQUAL)	NOT_FOUND(in);
	}
}
