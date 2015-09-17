#include <stdio.h>

int main(){
	int array[3][2]={{3,5},{4,6},{7,9}};
	//pointer to integer array
	int (*parray)[2]= array;
	for(int i=0; i<3;i++){
		for(int j=0;j<2;j++){
			printf("%d ",(*(parray+i))[j]);
	
}
		printf("\n");
}
	
	return 0;
}
