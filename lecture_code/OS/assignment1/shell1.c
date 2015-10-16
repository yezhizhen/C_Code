#include <glob.h>
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
//	printf("pid:%d not found in the job list!\n",pid_);
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

job* foreground(int index, jobs *jbs)
{
	job *temp=jbs->sentinel;
	int i=1;
	//while still has next
	while(temp=temp->next)
	{
		if(i==index)
		{
			printf("Job wake up: %s\n",temp->cm);
			tcsetpgrp(0,temp->pid);
			killpg(temp->pid,SIGCONT);
			//set
			return temp;
		}
		i++;
	}	
	printf("fg: no such job\n");
	return 0;
}

typedef struct
{
	int k;
	glob_t* pgl;
}wild;

wild wildexpand(char** tokens)
{
	int i=1;
	int flags=0;
	int k=0;
	int num;
	glob_t *pgl = malloc(sizeof(*pgl));
	//traverse all the tokens
	//while(tokens[i]&&strcmp(tokens[i],"")!=STRING_EQUAL)
	while(tokens[i]&&tokens[i]&&strcmp(tokens[i],"")!=STRING_EQUAL)
	{
		glob(tokens[i],flags,NULL,pgl);
		if(pgl->gl_pathc>0&&k==0)
		{
			flags=GLOB_APPEND;
			k = i;
		}
		i++;
	}
	//add all matched files
	for(num=0;num<pgl->gl_pathc;k++,num++)
	{
		tokens[k]=pgl->gl_pathv[num];	
		//printf("tokens[%d]:%s \n",k,tokens[k]);
	}
	wild a;
	a.k=k;
	a.pgl = pgl;
	//k is the final file index+1
	return a; 	
}

int notin(pid_t p,jobs* jbs)
{
	job *temp=jbs->sentinel;
	while(temp=temp->next)
	{
		if(temp->pid == p)
			return 0;
	}
	return 1;
}
/*int traversepipe(char **tokens)
{
	int i=0;
	while(tokens
}
*/
int length;
int main()
{
	//255 characters allowed
	pid_t parent_pid = getpid();
	int st;
	int i; 
	int isa;
	char buffer;
	int continueflag=0;
	char *in = malloc (sizeof(*in)*(MAXIMUM_P_SIZE));
	char* result_path = malloc(sizeof(*result_path)*260);
	char* cop = malloc(sizeof(*cop)*260);
	char *tokens[255];
	char* newtoken[70];
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
	signal(SIGTTIN,SIG_IGN);
	signal(SIGTTOU,SIG_IGN);
	int status;
	while(TRUE)
	{
		status = 0;
		child_pid=0;
		memset(in,0,MAXIMUM_P_SIZE);
		initialize(tokens);
		//basic prestring: path and shell name
		printf("\x1B[35m[My Shell:");

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
		strcpy(cop,in);
		//find how many process we need
		int num_pro=0;
		int ppp=0;
		int fds[2];
		newtoken[0] = strtok(cop,"|");
		int inp=0;
		//while newtoken is not NULL
		
		while(newtoken[num_pro])
		{
			newtoken[++num_pro] = strtok(NULL,"|");
			//printf("num:%d\n",num_pro);
		}
		/*for(;ppp<num_pro;ppp++)
		{
			printf("%d: %s\n",ppp,newtoken[ppp]);
		}*/
		//handle all except last process
		for(ppp=0;ppp<num_pro-1;ppp++)
		{
				pipe(fds);
				//handle the first
				if(ppp == 0)
				{
					//remember the first child_pid
					child_pid = fork();
					//handle children
					if(child_pid==0)
					{
						dup2(fds[1],1);
						close(fds[1]);
						//set the new input for child
						in = newtoken[ppp];
					}		
				}
				//handle others
				else
				{
					//children
					if(fork()==0)
					{
						in = newtoken[ppp];
						if(fds[1]!=1)
						{
							dup2(fds[1],1);
							close(fds[1]);
						}
						if(fds[0]!=0)
						{
							dup2(inp,0);
							close(inp);
						}
					}
				}
				//for parent
				inp = fds[0];
				close(fds[0]);
				close(fds[1]);
		}
		//handle the last
		if(num_pro != 1)
		{
			//for the last one
			if(fork==0)
			{
				in = newtoken[num_pro-1];
				dup2(inp,0);
				close(inp);
				//the parent still has stdout as the output, which is copied to the child
			}
		}	
		//try to get all parameters
		length = splitLine(in,tokens);
		wild m;
		m.k = -1;		
		if(length>1)	m = wildexpand(tokens);
		if(m.k>length)	length=m.k;
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
				setpgrp();
				tcsetpgrp(0,getpid());
				signal(SIGTTIN,SIG_IGN);
				signal(SIGTTOU,SIG_IGN);
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
				job* te;
				if(te = foreground(atoi(tokens[1]),&jbs))
					{
						child_pid = te->pid;
					}
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
				setpgrp();
				tcsetpgrp(0,getpid());
				signal(SIGINT,SIG_DFL);
				signal(SIGQUIT,SIG_DFL);
				signal(SIGTERM,SIG_DFL);
				signal(SIGTSTP,SIG_DFL);
				signal(SIGTTIN,SIG_IGN);
				signal(SIGTTOU,SIG_IGN);
				executeFile(result_path,"/bin/",tokens,in);
				executeFile(result_path,"/usr/bin/",tokens,in);
				executeFile(result_path,"./",tokens,in);
				exit(EXIT_FAILURE);
			}
		}
		//here is the parent
		if(child_pid!=0)
		{
			setpgid(child_pid,child_pid);
			tcsetpgrp(0,child_pid);
			waitpid(-child_pid,&status,WUNTRACED);
		}
		tcsetpgrp(0,getpid());
		printf("%d\n", getpid());
		//if child is stopped
		if(WIFSTOPPED(status))
		{
			printf("\n%d:suspended\n",child_pid);
			//foreground would be add.
			if(notin(child_pid,&jbs))
				add(&jbs, child_pid,in);	
		}
		if(WIFSIGNALED(status)||WIFEXITED(status))
		{
			if(WIFSIGNALED(status))
				printf("\n%d:terminated\n",child_pid);
				
			if(child_pid!=0)
			{
				delete(&jbs, child_pid);
				killpg(child_pid,SIGKILL);
			}
		}
		if(m.k!=-1)
			free(m.pgl);
		m.pgl=NULL;		
	}
	free(result_path);
	free(in);	
	free(cwd);
	return 0;
}

int splitLine(char *str, char** tokens)
{
	char *temp=malloc(sizeof(*temp)*260);
	strcpy(temp,str);
	int k=0;
	//printf("I'm naughty\n");
	//if not NULL
	tokens[k]=strtok(temp," \t");
	while(tokens[k])
	{
	//	printf("%d\n",k);
   		k++;
		tokens[k]=strtok(NULL," \t");
	}
	//printf("ha\n");
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
	tokens[length]=NULL;
	//printf("tokens[0]:%s\n",tokens[0]);
	if(execv(result_path,tokens) == -1) 
	{
		//save the errno
		int errsv = errno;
		//printf("errno:%d\n",errsv);
		if(errsv!=ENOENT)	printf("%s: unknown error\n",in);	
		else if(strcmp(path,"./")==STRING_EQUAL)	NOT_FOUND(in);
	}
}
