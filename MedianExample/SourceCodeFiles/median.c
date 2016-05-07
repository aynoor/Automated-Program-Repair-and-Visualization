#include <stdio.h>
int mid(int x,int  y, int z) {
	int m;
	m = z;
	if (y < z) {
		if (x <y) {
			m = y;
		} else if (x < z) {
			m = y;
		}
	} else {
		if (x > y) {
			m = y;
		} else if (x > z) {
			m = x;
		}
 	}
 	//printf("%s%d\n", "Middle number is: ", m);
 	return m;
}