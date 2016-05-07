#include <stdio.h>
#include "Statement.h"

void initialize_statement (Statement* s, int line_num) {
	s->_line_num = line_num;
}

void set_percentage_passed(Statement* s, double perc_passed) {
	s->_percentage_passed = perc_passed;
}

void set_percentage_failed(Statement* s, double perc_failed) {
	s->_percentage_failed = perc_failed;
}