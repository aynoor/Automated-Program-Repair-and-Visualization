#include "../SourceCodeFiles/median.c"
#include <stdio.h>
#include <string.h>
int main(int argc, const char** argv) {
	printf("%i\n", 1);
	if (3 == mid(3, 3, 5)) {
		printf("%s\n", "P");
	} else {
		printf("%s\n", " F");
	}
	return 0;
}
