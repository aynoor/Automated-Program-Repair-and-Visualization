#include "../SourceCodeFiles/median.c"
#include <stdio.h>
#include <string.h>

int main(int argc, const char** argv) {
	printf("%i\n", 4);
	if (5 == mid(5, 5, 5)) {
		printf("%s\n", "P");
	} else {
		printf("%s\n", "F");
	}
	return 0;
}