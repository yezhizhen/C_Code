#include <stdio.h>
#include <stdlib.h>

#define ROWS	10
#define COLS	8

//if use [][], wrong
void process_array(int** array) {
	int i, j, count = 0;
	for(i = 0; i < ROWS; i++)
		for(j = 0; j < COLS; j++){
			array[i][j] = count++;
			printf("%d\n",array[i][j]);
}
}

int main(void) {
	int **array = malloc(sizeof(int*) * ROWS * COLS);
	for(int i = 0; i < ROWS; i++)
	array[i]= malloc(sizeof(int)*COLS);
	
	process_array(array);
	printf("%d\n",array[0][0]);
	/*int i, j;
	for(i = 0; i < ROWS; i++) {
		for(j = 0; j < COLS; j++) {
			printf("%d ", array[i][j]);
		}
		printf("\n");
	}*/
	return 0;
}
