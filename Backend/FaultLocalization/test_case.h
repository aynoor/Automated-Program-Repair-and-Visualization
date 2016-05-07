#include <stdio.h>
#include "dynamic_array.c"

typedef struct {
	unsigned _id;
	char _status;
	Array _start_BB_line_nums;
} test_case;