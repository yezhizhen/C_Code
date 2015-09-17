#include <stdio.h>
#include <string.h>
union data{
	int age;
	char* name;
};

int main(){
	union data t;
	union data *pt;
	pt = &t;
	(*pt).age = 30;
	pt->name = "Euclid";
	printf("%d\n",strlen(pt->name));	
	printf("%s is %d years old\n",pt->name,pt->age);
	return 0;
}
