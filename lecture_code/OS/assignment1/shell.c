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
typedef struct job_{
	pid_t pid;
	struct job_ *next;
	char* cm;
}job;

typedef struct {
	job* sentinel;
	job* tail;
}jobs;

void add(jobs *jbs, int pid_,const char* command)
{
	job *new_jb = malloc(sizeof(job));
	new_jb->pid = pid_;
	new_jb->next = NULL;
	new_jb->cm = malloc(sizeof(char)*(strlen(command)+1));
	strcpy(new_jb->cm,command);
	jbs->tail->next = new_jb;
	jbs->tail= new_jb;
}

void delete(jobs *jbs, int pid_)
{
	job *pre = jbs->sentinel;
	job *find = pre->next;
	while(find)
	{
		if(find->pid==pid_)
		{
			pre->next = find->next;
			free(find);
			free(find->cm);
			find = 0;
			return;
		}
		pre = find;
		find = find->next;
	}
	printf("pid:%d not found in the job list!\n",pid_);
}

void listjobs(jobs *jbs)
{
	job *temp=jbs->sentinel;
	int i=1;
	//while still has next
	while(temp=temp->next)
	{
		printf("[%d] %s\n",i,temp->cm);
		i++;
	}
	if(i==1)	printf("No suspended jobs\n");
}

pid_t foreground(int index, jobs *jbs)
{
	job *temp=jbs->sentinel;
	int i=1;
	//while still has next
	while(temp=temp->next)
	{
		if(i==index)
		{
			kill(temp->pid,SIGCONT);
			printf("Job wake up: %s\n",temp->cm);
			return temp->pid;
		}
		i++;
	}	
	printf("fg: no such job\n");
	return 0;
}


int main()
{	
	//255 characters allowed
	int st;
	int i; 
	char buffer;
	int length;
	int continueflag=0;
	char *in = malloc (sizeof(*in)*(MAXIMUM_P_SIZE));
	char* result_path = malloc(sizeof(*result_path)*260);
	char *tokens[255];
	char* cwd = malloc(sizeof(*cwd)*200);	
	struct sigaction shell;
	sigset_t shellset = shell.sa_mask;
	sigemptyset(&shellset);
	shell.sa_handler = SIG_IGN;
	sigaction(SIGINT,&shell,NULL);
	sigaction(SIGTERM,&shell,NULL);
	sigaction(SIGQUIT,&shell,NULL);
	sigaction(SIGTSTP,&shell,NULL);
	jobs jbs; 
	//declare jobs
	jbs.sentinel = malloc(sizeof(job));
	jbs.sentinel->next=NULL;
	jbs.tail = jbs.sentinel; 
	pid_t child_pid;
	int status;
	while(TRUE)
	{
		status = 0;
		child_pid=0;
		//set all strings to null
		initialize(tokens);
		memset(in,0,MAXIMUM_P_SIZE);
		//basic prestring: path and shell name
		printf("[My Shell:");
		printf("%s",getcwd(cwd,200));
		printf("]$ ");
		fgets(in,MAXIMUM_P_SIZE,stdin);
		if(strlen(in)>0&&in[strlen(in)-1]!='\n')
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
			if(!(child_pid=fork()))
			{
				signal(SIGINT,SIG_DFL);
				signal(SIGQUIT,SIG_DFL);
				signal(SIGTERM,SIG_DFL);
				signal(SIGTSTP,SIG_DFL);
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
				//if still has job
				if(jbs.sentinel->next)
				{
					printf("There is at least one suspended job\n");
				}
				else
				{
					exit(EXIT_SUCCESS);
				}
			}
			else	printf("exit: wrong number of arguments\n");
		}
		//deal with jobs command
		else if(strcmp(tokens[0],"jobs")==STRING_EQUAL)
		{
			listjobs(&jbs);
		}
		else if(strcmp(tokens[0],"fg")==STRING_EQUAL)
		{
			if(length==2)
			{
				child_pid = foreground(atoi(tokens[1]),&jbs);
			}
			else
			{
				printf("fg: wrong number of arguments\n");
			}
		}
		//deal with file name input
		else
		{
			//child process
			if((child_pid=fork())==0)
			{
				signal(SIGINT,SIG_DFL);
				signal(SIGQUIT,SIG_DFL);
				signal(SIGTERM,SIG_DFL);
				signal(SIGTSTP,SIG_DFL);
				shell.sa_handler = SIG_DFL;
				executeFile(result_path,"/bin/",tokens,in);
				executeFile(result_path,"/usr/bin/",tokens,in);
				executeFile(result_path,"./",tokens,in);
				exit(EXIT_FAILURE);
			}
		}
		//here is the parent
		if(child_pid!=0)
			waitpid(child_pid,&status,WUNTRACED);
		//if child is stopped
		if(WIFSTOPPED(status))
		{
			printf("\n%d:suspended\n",child_pid);
			add(&jbs, child_pid,in);	
		}
		if(WIFSIGNALED(status))
		{
			printf("\n%d:terminated\n",child_pid);
			delete(&jbs, child_pid);
		}		
	}
	free(result_path);
	free(tokens);
	free(in);	
	free(cwd);
	return 0;
}

int splitLine(char *str, char** tokens)
{
	char *temp=malloc(sizeof(*temp)*260);
	strcpy(temp,str);
	int k=0;
	tokens[0] = strtok(temp," \t");    		
	while(tokens[k]!=NULL)
	{
   		k++;
    	tokens[k] = strtok(NULL," ");
	}
	return k;
	free(temp);
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
	int i=1;
	//traverse all the tokens
	while(tokens[i])
	{
		for(int t=0;t<strlen(tokens[i]);t++)
		{
			
		}
		i++;
	}
	if(execv(result_path,tokens) == -1) 
	{
		//save the errno
		int errsv = errno;
	//	printf("errno:%d\n",errsv);
		if(errsv!=ENOENT)	printf("%s: unknown error\n",in);	
		else if(strcmp(path,"./")==STRING_EQUAL)	NOT_FOUND(in);
	}
}
