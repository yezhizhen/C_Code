#include <stdio.h>

int * addition(int a, int b) {
	int c = a + b;
	int *d = &c;
	return d;
}

int main(void) {
	int result = *(addition(1, 2));
	int *result_ptr = addition(1, 2);
	printf("result = %d\n", result);
	printf("result = %d\n", *result_ptr);
	return 0;
}
