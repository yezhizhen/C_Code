#include <stdio.h>
#include <stdlib.h>
#define length 6


void mergeSort(int arr[],int start,int end);
void merge(int arr[],int start,int end);
int main(){
	int a[length]={3,2,7,4,5,1};
	mergeSort(a,0,length-1);
	for(int i=0;i<length;i++){
		printf("%d\n",a[i]);
}
/*	int b[length]={1,3,5,2,4,6};
	merge(b,0,length-1);
	printf("b:\n");
	for(int i=0;i<length;i++){
		printf("%d\n",b[i]);
}*/

	return 0;
}

void mergeSort(int arr[],int start,int end){
	if(start==end)
		return;

	mergeSort(arr,start,(start+end)/2);
	mergeSort(arr,(start+end)/2+1,end);
	merge(arr,start,end);
	
}

void merge(int arr[],int start,int end){
	const int middle=(start+end)/2;
	const int size1= middle-start+1;
	const int size2= end-middle;
	int *subarray1= malloc(sizeof(*subarray1)*(size1));
	int *subarray2= malloc(sizeof(*subarray1)*(size2));
	int *start1 = subarray1;
	int *start2 = subarray2;
	for(int i=0; i<size1;i++){
		subarray1[i]=arr[start+i];
}
	for(int k=0; k<size2;k++){
		subarray2[k]= arr[middle+1+k];
}

	for(int p=start; p<=end;p++){
		if(subarray2-start2>=size2){
			arr[p]= *(subarray1++);
			continue;
}
		if(subarray1-start1>=size1){
			arr[p]= *(subarray2++);
			continue;
}
		if(*subarray1>*subarray2){
			arr[p] = *(subarray2++);
}
		else{
			arr[p] = *(subarray1++);
}
		 
}
	free(start1);
	free(start2);	
}
