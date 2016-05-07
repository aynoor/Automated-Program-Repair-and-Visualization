#include "../SourceCodeFiles/average.c"
#include <stdio.h>
#include <string.h>

int main(int argc, const char** argv) {
	printf("%i\n", 5);
	int array[] = {10, 250, 35, 400, 65};
	if (152 == average(array)) {		
		printf("%s\n", "P");
	} else {
		printf("%s\n", "F");
	}
	return 0;
}
