#include <stdio.h>
#include <string.h>
struct data{
	int age;
	char* name;
};

int main(){
	struct data t;
	struct data *pt;
	pt = &t;
	(*pt).age = 30;
	pt->name = "Euclid";
	printf("%d\n",strlen(pt->name));	
	printf("%s is %d years old\n",pt->name,pt->age);
	return 0;
}
