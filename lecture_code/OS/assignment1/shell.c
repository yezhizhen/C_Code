#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#define TRUE 1
#define STRING_EQUAL 0
#define MAXIMUM_SIZE 255
#define MAXIMUM_P_SIZE 256
#define NOT_FOUND(in) printf("%s: command not found\n",in)


int main()
{
	//255 characters allowed
	char *in = malloc (sizeof(*in)*(MAXIMUM_P_SIZE));
	while(TRUE)
	{
		//basic prestring: path and shell name
		printf("[My Shell:");
		printf("%s",getenv("PWD"));
		printf("]$ ");
		//reading input
		//fgets(in,MAXIMUM_P_SIZE,stdin);
		scanf("%255s", in);
		//switch case		
		if(strcmp(in,"help") == STRING_EQUAL)
		{
			printf("List of functions you need\n");
		}
		else if(*in=='/')
		{
			//create and run child in if
			if(!fork())
			{
				if(execl(in,in,NULL) == -1) NOT_FOUND(in);	
			}
			//parent wait for the child
			wait(NULL);
		}
		else NOT_FOUND(in); 
	}


	return 0;
}
