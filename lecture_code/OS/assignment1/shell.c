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

char** splitLine(char* str);
void executeFile(char** tokens,int argc);

int main()
{
	//255 characters allowed
	char *in = malloc (sizeof(*in)*(MAXIMUM_P_SIZE));
	int i; 
	char buffer;
	int continueflag=0;
	int k=0;
	int position=0;
	char* result_path = malloc(sizeof(*result_path)*280);
	char** tokens = malloc(sizeof(*tokens)*255);
	
	while(TRUE)
	{
		position = 0;
		memset(tokens,0,255*sizeof(*tokens));
		//basic prestring: path and shell name
		printf("[My Shell:");
		printf("%s",getenv("PWD"));
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
		/*for(i=0;i<=256;i++)
		{
			if(in[i]=='\0') 
			{
				if(i==256)
				{ 
					printf("Exceed maximum length 255\n");
					//clear the input buffer
					while((buffer=getchar())!='\n'&& buffer != EOF);
					continueflag = TRUE;	
					break;	
				}
				else break;
			}
		}
		*/
		if(continueflag)
		{
			continue;
		}
		//try to get all parameters
		//tokens[0] = strtok(in,' ');
		//while(tokens[k]!=NULL)
		//{
		//	k++;
		//	tokens[k] = strtok(NULL,' ');
		//}
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
				if(execl(in,in,NULL) == -1) 
				{
				//save the errno
				int errsv = errno;
				//printf("error number is%d\n",errsv);								
				if(errsv==ENOENT)	NOT_FOUND(in);
				else printf("%s: unknown error",in);	
				}
			}
			//parent wait for the child
			wait(NULL);
		}
		//deal with file name input
		else
		{
			//child process
			if(fork()==0)
			{
				strcpy(result_path,"/bin/");
				strcat(result_path, in);
				if(execl(result_path,result_path,NULL) == -1) 
				{
				//save the errno
				int errsv = errno;
				if(errsv!=ENOENT)	 printf("%s: unknown error",in);	
				}

				strcpy(result_path,"/usr/bin/");
				strcat(result_path, in);				
				if(execl(result_path,result_path,NULL) == -1) 
				{
				int errsv = errno;
				if(errsv!=ENOENT)	 printf("%s: unknown error",in);	
				}

				strcpy(result_path,"./");
				strcat(result_path, in);
				if(execl(result_path,result_path,NULL) == -1) 
				{
				int errsv = errno;
				//printf("error number is%d\n",errsv);								
				if(errsv!=ENOENT)	 printf("%s: unknown error",in);	
				else	NOT_FOUND(in);
				}
				
			}
			wait(NULL);
		}
	}
	free(result_path);
	free(tokens);
	free(in);	
	return 0;
}

char** splitLine(char *str)
{
	

	
}




