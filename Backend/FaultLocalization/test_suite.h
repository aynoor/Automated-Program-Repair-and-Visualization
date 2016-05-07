#include <stdio.h>
#include "test_case.c"

typedef struct {
	test_case* _test_cases;
	size_t _size;
	size_t _used;
	int _total_passed_tc;
	int _total_failed_tc;
} test_suite;