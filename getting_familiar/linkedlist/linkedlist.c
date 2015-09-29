#include <stdio.h>
#include "linkedlist.h"
#include <stdlib.h>



int main()
{
	node *head = NULL;
	int number;
	do
	{
		scanf("%d",&number);
		if(number==-1)	break;
		//ELSE
		node *p = malloc(sizeof(*p));
		p->value = number;
		p->next = NULL;
		node *last = head;
		if(last)
		{
			while(last->next)
			{
				last = last->next;
			}
			last->next = p;
		}
		else
		{
			head = p;
		}
	}while(number!=-1);

	return 0;
}
