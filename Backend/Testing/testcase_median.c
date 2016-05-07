#include "median.c"
#include <stdio.h>
#include <string.h>

int main(int argc, const char** argv) {
	//int[] values[6] = {{3,3,5}, {1,2,3}, {3,2,1}, {5,5,5}, {2,1,3}};
	/*for (int i = 0; i < 6; ++i) {
		printf("%i\n", i+1);
		int temp_values[3] = values[i];
		if (3 == mid(temp_values[0], temp_values[1], temp_values[2])) {
			printf("P\n");
		} else {
			printf("F\n");
		}
	}*/

	printf("%i\n", 1);
	if (3 == mid(3, 3, 5)) {
		printf("P\n");
	} else {
		printf("F\n");
	}

	printf("%i\n", 2);
	if (2 == mid(1, 2, 3)) {
		printf("P\n");
	} else {
		printf("F\n");
	}

	printf("%i\n", 3);
	if (2 == mid(3, 2, 1)) {
		printf("P\n");
	} else {
		printf("F\n");
	}

	printf("%i\n", 4);
	if (5 == mid(5, 5, 5)) {
		printf("P\n");
	} else {
		printf("F\n");
	}
	printf("%i\n", 5);
	printf("%d\n", mid(5, 3, 4));
	if (4 == mid(5, 3, 4)) {

		printf("P\n");
	} else {
		printf("F\n");
	}

	printf("%i\n", 6);
	if (2 == mid(2, 1, 3)) {
		printf("P\n");
	} else {
		printf("F\n");
	}
	return 0;
}