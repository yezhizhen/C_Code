#include <stdio.h>
#define SUCCESS 0
#define PRINTLN(x)  printf(x); return putchar('\n')
#define FAILURE 1


int main(){
	int a;
	a = PRINTLN("This is Macro!");
	if(a!=EOF)
		return SUCCESS;
	else
		return FAILURE;
}
