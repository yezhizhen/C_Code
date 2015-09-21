#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
int main()
{
//   execl("/bin/ls","/bin/ls","-l",NULL); 
	int i=0;
	char **str=malloc(sizeof(*str)*50);
	memset(str,0,50);

	str[0]="/bin/ls";
	str[1]="-l";
	for(i=0;str[i]!=NULL;i++)
	{
		printf("str[%d]:%s\n",i,str[i]);


	}		
	execv(str[0],str);	
	return(0);

}
