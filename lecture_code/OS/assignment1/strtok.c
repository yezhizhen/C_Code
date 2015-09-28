#include <stdio.h>
#include <string.h>

int main()
{
	char str[40]="abc    afde fahoivbas";
	char *token;
	char *tokens[6];
	int i=0;
	printf("str:%s\n",str);
	token = strtok(str," ");
	while(token!=NULL)
	{
		tokens[i] = token;
		printf("tokens[%d]: %s\n",i,tokens[i]);
		token = strtok(NULL," ");
		i++;	
	}
	printf("str:%s\n",str);










	return 0;
}
