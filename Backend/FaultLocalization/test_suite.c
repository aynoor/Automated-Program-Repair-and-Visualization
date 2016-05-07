#include "test_suite.h"

void initialize_test_suite(test_suite* ts, size_t initial_size) {
	ts->_size = initial_size;
	ts->_used = 0;
	ts->_test_cases = ( test_case *)malloc( sizeof(test_case) * initial_size);
}

void insert_test_suite(test_suite* ts, test_case tc) {
	if (ts->_used == ts->_size) {
		ts->_size = ts->_size*2;
		ts->_test_cases = (test_case*)realloc(ts->_test_cases, sizeof(test_case) * ts->_size );
	}
	ts->_test_cases[ts->_used++] = tc;
}

void free_test_suite(test_suite *ts) {
 	free(ts->_test_cases);
 	ts->_used = 0;
 	ts->_size = 0;
 	ts->_test_cases = NULL;
}

void print_test_suite(test_suite* ts) {
	int i = 0;
	for (i = 0; i < ts->_used; ++i) {
		printf("------------------------------------------------------------------------\n");
		test_case* tc = &ts->_test_cases[i];
		print_test_case(tc);
		printf("------------------------------------------------------------------------\n");
	}
	
}

void num_of_failed_test_cases(test_suite* ts) {
	int i = 0;
	ts->_total_failed_tc = 0;
	for (i = 0; i < ts->_used; ++i) {
		test_case* tc = &ts->_test_cases[i];
		if (tc->_status == 'F') {
			++ts->_total_failed_tc;
		}
	}
}

void num_of_passed_test_cases(test_suite* ts) {
	int i = 0;
	ts->_total_passed_tc = 0;
	for (i = 0; i < ts->_used; ++i) {
		test_case* tc = &ts->_test_cases[i];
		if (tc->_status == 'P') {
			++ts->_total_passed_tc;
		}
	}
}

int num_of_failed_tc_executing_s(test_suite* ts, int line) {
	int i = 0;
	int num_failed_tc_executing_s = 0;
	for (i = 0; i < ts->_used; ++i) {
		test_case* tc = &ts->_test_cases[i];
		if (tc->_status == 'F') {
			int j = 0;
			for (j = 0; j < tc->_start_BB_line_nums.used; ++j) {
				if (line == tc->_start_BB_line_nums.array[j]) {
					++num_failed_tc_executing_s;
					break;
				}
			}
		}
	}
	return num_failed_tc_executing_s;
}


int num_of_passed_tc_executing_s(test_suite* ts, int line) {
	int i = 0;
	int num_pass_tc_executing_s = 0;
	for (i = 0; i < ts->_used; ++i) {
		test_case* tc = &ts->_test_cases[i];
		if (tc->_status == 'P') {
			int j = 0;
			for (j = 0; j < tc->_start_BB_line_nums.used; ++j) {
				if (line == tc->_start_BB_line_nums.array[j]) {
					++num_pass_tc_executing_s;
					break;
				}
			}
		}
	}
	return num_pass_tc_executing_s;
}
float percentage_passed_s(test_suite* ts, int line) {
	//printf("Num of passed test cases executing s: %d\n", num_of_passed_tc_executing_s(ts, line));
	//printf("Total passed test cases: %d\n", ts->_total_passed_tc);
	float total_passed = (float)ts->_total_passed_tc;	// I WAS ADDED
	if (total_passed == 0.0) return 0.0;
	return ((float) num_of_passed_tc_executing_s(ts, line))/ total_passed;
}

double percentage_failed_s(test_suite* ts, int line) {
	//printf("Number of failed test cases executing s: %d\n", num_of_failed_tc_executing_s(ts, line));
	//printf("Total failed test cases: %d\n", ts->_total_failed_tc);
	float total_passed = (float) ts->_total_failed_tc;		// I WAS ADDED
	if (total_passed == 0.0) return 0.0;
	return ((float) num_of_failed_tc_executing_s(ts, line))/ total_passed;
}
