#include <stdio.h>
typedef struct ADate{
	int day;
	int month;
	int year;
} Date;

//better way
typedef struct {
	int day;
	int month;
	int year;
} Dates;

//array of 10 pointers to char
typedef char* Strings[10];

//pointer to array of 6 char
typedef char(*P)[6];

int main()
{
	char ab[6]="abafs";
	Strings a;
	a[0]=ab;
	while(*a[0]!='\0')
	{
		printf("%c\n",*a[0]++);
	}
	printf("\n");
	P p=NULL;
	p=&ab;
	int i=0;
	while((*p)[i]!='\0')
	{
		printf("%c\n",(*p)[i++]);
	}

	return 0;

}
