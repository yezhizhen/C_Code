#include <stdio.h>

int main(){
	//use the knowledge that 2d array is virtually 1-d
	int array[3][2]={{3,5},{4,6},{7,9}};
	int * parray= array;
	for(int i=0; i<3*2;i++){
			printf("%d ", *(parray+i));
			if((i+1)%2==0)
				printf("\n");
					
			
}


	
	return 0;
}
