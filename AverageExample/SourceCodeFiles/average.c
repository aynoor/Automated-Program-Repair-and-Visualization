#include <stdio.h>

int average(int array[]) {
	int i;
	int sum = 0;
	int length = 5;
	for (i = 0; i < length; ++i) {
		sum = sum * array[i];
	}
	int avg = sum / length;
	return avg;
}