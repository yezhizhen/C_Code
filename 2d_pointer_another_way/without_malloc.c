#include <stdio.h>
#include <stdlib.h>
//segmentation fault
int main(){
	int a[3][2]={{0,1},{2,3},{3,4}};	
	int** pp;/*= malloc (sizeof(int *)*3);
	for(int r=0;r<3;r++){
		pp[r]= malloc(sizeof(int)*2);
}
*/
	//initialize
	for(int t=0;t<3;t++){
		pp[t]=a[t];	
}	
	//print	
	for(int i=0;i<3;i++){
		for(int j=0;j<2;j++){
			printf("%d ",pp[i][j]);
}
		printf("\n");
}
	a[2][1]=5;
	printf("hello%d\n",pp[2][1]);
	return 0;
}






