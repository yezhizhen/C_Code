#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#define TRUE 1
#define STRING_EQUAL 0
#define MAXIMUM_SIZE 255
#define MAXIMUM_P_SIZE 256
#define NOT_FOUND(in) printf("%s: command not found\n",in)

int splitLine(char* str,char** tokens);
void executeFile(char** tokens,int argc);
void cdfunction(char** tokens, int length);
void initialize(char** tokens);
int main()
{
	//255 characters allowed
	char *in = malloc (sizeof(*in)*(MAXIMUM_P_SIZE));
	int i; 
	char buffer;
	int length;
	int continueflag=0;
	int position=0;
	char* result_path = malloc(sizeof(*result_path)*280);
	char** tokens = malloc(sizeof(*tokens)*MAXIMUM_SIZE);
	char* cwd = malloc(sizeof(*cwd)*200);	
	while(TRUE)
	{
		position = 0;
		//set all strings to null
		initialize(tokens);
		memset(in,'\0',MAXIMUM_P_SIZE);
		//basic prestring: path and shell name
		printf("[My Shell:");
		printf("%s",getcwd(cwd,200));
		printf("]$ ");
		//reading input and check if exceeds limit
		//if(scanf("%256s", in) == EOF) continue;
		while((buffer=getchar())!='\n'&&buffer!=EOF)
		{	
			if(position==255)
			{
			printf("Exceed maximum length 255\n");
			while((buffer=getchar())!='\n'&& buffer != EOF);
			continueflag = TRUE;
			break;
			}
			in[position++]=buffer;
		}
		//if empty string 
		if(!position) continue; 
		if(continueflag)	continue;
		
		//try to get all parameters
		length = splitLine(in,tokens);			
		//switch case		
		if(strcmp(in,"help") == STRING_EQUAL)
		{
			printf("List of functions you need\n");
		}
		//else if(*in=='') continue;
		else if(*in=='/'||*in=='.')
		{
			//create and run child in if
			if(!fork())
			{
				if(execv(tokens[0],tokens) == -1) 
				{
				//save the errno
				int errsv = errno;
				//printf("error number is%d\n",errsv);								
				if(errsv==ENOENT)	NOT_FOUND(in);
				else printf("%s: unknown error",in);	
				}
			}
			//parent wait for the child
		}
		//deal with cd comand
		else if(strcmp(tokens[0],"cd")==STRING_EQUAL)
		{
			cdfunction(tokens,length);
		}
		//deal with file name input
		else
		{
			//child process
			if(fork()==0)
			{
				strcpy(result_path,"/bin/");
				strcat(result_path, tokens[0]);
				tokens[0] = result_path;
				if(execv(result_path,tokens) == -1) 
				{
				//save the errno
				int errsv = errno;
				if(errsv!=ENOENT)	 printf("%s: unknown error",in);	
				}

				strcpy(result_path,"/usr/bin/");
				strcat(result_path, tokens[0]);
				tokens[0] = result_path;				
				if(execv(result_path,tokens) == -1) 
				{
				int errsv = errno;
				if(errsv!=ENOENT)	 printf("%s: unknown error",in);	
				}

				strcpy(result_path,"./");
				strcat(result_path, tokens[0]);
				tokens[0] = result_path;
				if(execv(result_path,tokens) == -1) 
				{
				int errsv = errno;
				//printf("error number is%d\n",errsv);								
				if(errsv!=ENOENT)	 printf("%s: unknown error",in);	
				else	NOT_FOUND(in);
				}
				
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
		tokens[i]=0;
	}	
}
