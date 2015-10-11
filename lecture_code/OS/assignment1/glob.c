#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <glob.h>
int main(int argc, char **argv)
{
	glob_t *pgl = malloc(sizeof(*pgl));
	int flags=0;
	for(int i=1;i<argc;i++)
	{
		flags=(i>1)?GLOB_APPEND:0;
		glob(argv[i],flags,NULL,pgl);
	}
	
	for(int i=0;i<pgl->gl_pathc;i++)
	{
		printf("%d:%s\n",i,pgl->gl_pathv[i]);
	}
	globfree(pgl);
}
