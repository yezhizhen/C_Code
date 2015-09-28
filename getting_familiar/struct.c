#include <stdio.h>
#include <string.h>
struct data{
	int age;
	char* name;
}t,p;


//anonymous
struct{
	int x;
	int y;
}p1,p2;

struct point{
	int x;
	int y;
};

/*
struct point getStruct()
{
	
}

struct point* getStruct(struct point *s)
{
	
}


*/
int main()
{
	struct data m={20,"Jenny"};
	struct data *pt;
	pt = &t;
	p = (struct data){5,"fava"};
	(*pt).age = 30;
	pt->name = "Euclid";
	printf("length of name:%zd\n",strlen(pt->name));	
	printf("%s is %d years old\n",pt->name,pt->age);
	printf("length of name:%zd\n",strlen(m.name));	
	printf("%s is %d years old\n",m.name,m.age);
	p=t;
	return 0;
}
