#include <stdio.h>
#include "linkedlist.h"
#include <stdlib.h>

//with sentinal
//
//
//promotes evolvability

void add(List *l, const int num)
{
		node *p = malloc(sizeof(*p));
		p->pvalue = malloc(sizeof(*(p->pvalue)));
		*(p->pvalue) = num;
		p->next = l->sentinal;
		l->tail->next = p;
		l->tail = p;
}

void printall(List* list)
{
	node *p;
	for(p=list->sentinal->next;p->pvalue;p=p->next)
	{
		printf("%d\t",*(p->pvalue));
	}
}

void delete(List* list, int number)
{
	node *p = list->sentinal->next;	
	
	node *back = list->sentinal;
	//every time use ->, check if pointer is NULL
	for(;p->pvalue;back = p,p=p->next)
	{
		if(*(p->pvalue)==number)
		{
			back->next = p->next;
			free(p);
			break;
		}
	}
}

void clean(List *list)
{
	node *p;
	node *q;
	for(p=list->sentinal->next,q=p;p->pvalue;p = q)
	{
		q = q->next;
		free(p);
	}
	//No need to free the sentinal
	
}

int main()
{
	List list;
	node *sen = list.sentinal;
	sen->pvalue = NULL;
	sen->next = sen;
	list.tail = sen;
	//list.head = NULL;
	int number;
	do
	{
		scanf("%d",&number);
		if(number==-1)	break;
		//ELSE
		add(&list,number);
	}while(number!=-1);
	delete(&list,3);	
	//classical traversal
	printall(&list);
	clean(&list);
	return 0;
}


