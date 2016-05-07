#include "../SourceCodeFiles/average.c"
#include <stdio.h>
#include <string.h>

int main(int argc, const char** argv) {
	printf("%i\n", 1);
	int array[] = {1, 2, 3, 4, 5};
	if (3 == average(array)) {
		printf("%s\n", "P");
	} else {
		printf("%s\n", "F");
	}
	return 0;
}
