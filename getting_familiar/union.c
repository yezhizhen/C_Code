#include <stdio.h>
#include <string.h>
union data{
	int age;
	char* name;
};

typedef union{
	int a;
	char c[sizeof(int)];
} bt;

int main(){
/*	union data t;
	union data *pt;
	pt = &t;
	(*pt).age = 30;
	pt->name = "Euclid";
	printf("%zd\n",strlen(pt->name));	
	printf("%s is %d years old\n",pt->name,pt->age);
*/
	bt un;
	un.a=0xcf03;
	int i;
	for(i=0;i<sizeof(int);i++)
	{
		printf("%02hhX",un.c[i]);
	}
	putchar('\n');
	return 0;
}
