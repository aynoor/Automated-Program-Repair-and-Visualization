#include <stdio.h>
#include "test_case.h"

void initialize_test_case (test_case* tc, unsigned id, size_t size_of_array) {
	tc->_id = id;
	initialize_array(&tc->_start_BB_line_nums, size_of_array); 
}

void set_test_case_status(test_case* tc, char status) {
	tc->_status = status;
}

void insert_BB_line_num(test_case* tc, int element) {
	insert_array(&tc->_start_BB_line_nums, element);
}

void print_test_case(test_case* tc) {
	printf("%d\n", tc->_id);
	printf("%c\n", tc->_status);
	int i = 0;
	Array* _arr = &tc->_start_BB_line_nums;					// turn a value into a pointer with &
	for (i = 0; i < _arr->used; ++i) {
        printf("%d\n", _arr->array[i]);
	}
}

void free_test_case(test_case* tc) {
	free_array(&tc->_start_BB_line_nums);
	tc->_id = 0;
	tc->_status = '\0';
}