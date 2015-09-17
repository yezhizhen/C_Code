#include <stdio.h>

int main(int argc, char** argv){
	int c;
	for(c=0;c<argc;c++){
		printf("argv %d: %s\n",c, *(argv++));
}
	return 0;
}
