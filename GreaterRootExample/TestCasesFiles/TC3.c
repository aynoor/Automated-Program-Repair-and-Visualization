#include "../SourceCodeFiles/greater_root.c"
#include <stdio.h>
#include <string.h>

int main(int argc, const char** argv) {
	printf("%i\n", 3);
	int a = 1;
	int b = 4;
	int c = 4;
	if (-2 == greater_root(a,b,c)) {
		printf("%s\n", "P");
	} else {
		printf("%s\n", "F");
	}
	return 0;
}
