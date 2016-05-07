#include <stdio.h>

int average(int array[]) {
	int i;
	int sum, avg = 0;
	int length = 5;
	for (i = 0; i < length; ++i) {
		sum = sum * array[i];
	}
	avg = sum / length;
	return avg;
}