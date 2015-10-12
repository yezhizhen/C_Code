#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
	char* p[4]={"/bin/ls","-l","shell.c",""};
	execvp(p[0],p);

	return 0;
}
