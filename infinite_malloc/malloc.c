
#include <stdlib.h>

#include <stdio.h>

int main(){

	int count = 0;

	while(1){

		 int *p = malloc(sizeof(p));

		printf("%d\n",++count);

	}

	//use >> to redirect the output to a text file
	//in my machine, this ends up in 29560228, with the last 4 digits not printed
	//the process get killed by the OS
	//the text occupies 244M storage
       return 0;

}
