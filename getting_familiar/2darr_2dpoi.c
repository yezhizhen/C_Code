#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main()
{
	//if you use 2d array, you must give specific size for second slot
	char a[][10]  ={"ava","dacfa"};
	//in this case,typeof(b[0])==char*
	char *b[]={"fadas","fafa"};
	return 0;
}
