#include <stdio.h>
typedef struct ADate{
	int day;
	int month;
	int year;
} Date;

//better way
typedef struct {
	int day;
	int month;
	int year;
} Dates;

//
typedef char[10] Strings;

int main()
{
	return 0;

}
