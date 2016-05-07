#include "../SourceCodeFiles/average.c"
#include <stdio.h>
#include <string.h>

int main(int argc, const char** argv) {
	printf("%i\n", 2);
	int array[] = {0, 0, 0, 0, 0};
	if (0 == average(array)) {
		printf("%s\n", "P");
	} else {
		printf("%s\n", "F");
	}
	return 0;
}
