#include "../SourceCodeFiles/average.c"
#include <stdio.h>
#include <string.h>

int main(int argc, const char** argv) {
	printf("%i\n", 4);
	int array[] = {110, 25, 35, 40, 55};
	if (53 == average(array)) {
		printf("%s\n", "P");
	} else {
		printf("%s\n", "F");
	}
	return 0;
}
