#include <stdio.h>
#include <algorithm>
#include <math.h>

void findsolution(int *a, int num);
using namespace std;
void printope(int op);
int operate(int *a);
void operates(int *a,int ope, int index, double *result);
//enum oper{add,minus,times,divide};
int main()
{
	int a[4];
	int i=0;	
	while(1)
	{
		while(1)
		{
			scanf("%d %d %d %d",&a[0],&a[1],&a[2],&a[3]);
			findsolution(a,24);
		}

	}
	return 0;
}

void findsolution(int *a, int num)
{
	sort(a,a+4);
	do
	{
		if(operate(a))
			break;
		//printf("%d %d %d %d\n",a[0],a[1],a[2],a[3]);
	}while(next_permutation(a,a+4));
	

//	printf("After loop:%d %d %d %d\n",a[0],a[1],a[2],a[3]);
}

int operate(int *a)
{
	double result =0;
	int f[4]={0,1,2,3};
	for(int l1=0;l1<4;l1++)
		for(int l2=0;l2<4;l2++)
			for(int l3=0;l3<4;l3++)
			{
				result = a[0];
				operates(a,l1,0,&result);
				operates(a,l2,1,&result);
				operates(a,l3,2,&result);
				if(fabs(24-result)<1E-3)
				{
					printf("%d",a[0]);
					printope(l1);
					printf("%d",a[1]);
					printope(l2);
					printf("%d",a[2]);
					printope(l3);
					printf("%d",a[3]);
					printf("\n");
					return 1;
				}
			}
	return 0;
}

void operates(int *a,int ope, int index, double *result)
{

	switch(ope)
	{
		case 0:
			*result += a[index+1];
			break;
		case 1:
			*result -= a[index+1];
			break;
		case 2:
			*result *= a[index+1];
			break;
		case 3:
			*result /= a[index+1];
			break;			
	}
}

void printope(int op)
{	
	switch(op)
	{
		case 0:
			printf("+");
			break;
		case 1:
			printf("-");
			break;
		case 2:
			printf("*");
			break;
		case 3:
			printf("/");
			break;			
	}
}
