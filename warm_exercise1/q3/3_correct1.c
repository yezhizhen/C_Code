#include <stdio.h>
#include <stdlib.h>

#define ROWS	10
#define COLS	8

void process_array(int array[ROWS][COLS]) {
	int i, j, count = 0;
	for(i = 0; i < ROWS; i++)
		for(j = 0; j < COLS; j++)
			array[i][j] = count++;
}

int main(void) {
	int array[ROWS][COLS];
	process_array(array);
	int i, j;
	for(i = 0; i < ROWS; i++) {
		for(j = 0; j < COLS; j++) {
			printf("%d ", array[i][j]);
		}
		printf("\n");
	}
	return 0;
}
