#include "../SourceCodeFiles/median.c"
#include <stdio.h>
#include <string.h>

int main(int argc, const char** argv) {
	printf("%i\n", 6);
	if (2 == mid(2, 1, 3)) {
		printf("%s\n", "P");
	} else {
		printf("%s\n", "F");
	}
	return 0;
}