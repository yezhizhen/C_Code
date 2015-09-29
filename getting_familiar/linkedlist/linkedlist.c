#include <stdio.h>
#include "linkedlist.h"
#include <stdlib.h>

//promotes evolvability

void add(List *l, int num)
{
		node *p = malloc(sizeof(*p));
		p->value = num;
		p->next = NULL;
		if(l->tail)
		{
			l->tail->next = p;
		}
		else
		{
			l->head = p;
		}
		l->tail = p;
}

int main()
{
	List list;
	list.head = NULL;
	list.tail = NULL;
	int number;
	do
	{
		scanf("%d",&number);
		if(number==-1)	break;
		//ELSE
		add(&list,number);
	}while(number!=-1);

	return 0;
}
