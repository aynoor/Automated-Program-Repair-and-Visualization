#include <stdio.h>
#include "Statements.h"

void initialize_statements(Statements* ss, size_t initial_size) {
	ss->_size = initial_size;
	ss->_used = 0;
	ss->_statements = ( Statement *)malloc( sizeof(Statement) * initial_size);
}


void insert_statement(Statements* ss, Statement s) {
	if (ss->_used == ss->_size) {
		ss->_size = ss->_size*2;
		ss->_statements = (Statement*)realloc(ss->_statements, sizeof(Statement) * ss->_size );
	}
	ss->_statements[ss->_used++] = s;
}

void free_statements(Statements *ss) {
 	free(ss->_statements);
 	ss->_used = 0;
 	ss->_size = 0;
 	ss->_statements = NULL;
}

void print_statements(Statements* ss) {
	int i = 0;
	printf("------------------------------------------------------------------------\n");
	printf("Statements with their percentages.\n");
	for (i = 0; i < ss->_used; ++i) {
		Statement* s = &ss->_statements[i]; 
		printf("%d%s%.3f%s%.3f\n", s->_line_num, " %passeed: ", s->_percentage_passed, " %failed: ", s->_percentage_failed);
	}
}

double get_suspiciousness(double percentage_failed, double percentage_passed) {
	double total_percentage = percentage_failed+percentage_passed;
	if (total_percentage > 0.0) {
		return percentage_failed/total_percentage;
	} else {
		//printf("%s%f%s%f%s%f\n", "total per: ", total_percentage, "per failed: ", percentage_failed, "per passed: ", percentage_passed );
		return -1.0;										// if problem check this ans -mac_color_value
	}
}

void print_color_range(Statements* ss) {
	int i = 0;
	printf("%s\n", "Color of Statements: ");
	int color_red = 120;
	for (i = 0; i < ss->_used; ++i) {
		Statement* s = &ss->_statements[i]; 
		printf("%d%s%f\n", s->_line_num, " ", get_suspiciousness((s->_percentage_failed), s->_percentage_passed)*color_red);
	}
}

void print_suspiciousness(Statements* ss) {
	int i = 0;
	printf("------------------------------------------------------------------------\n");
	printf("Suspiciousness of Statements.\n");
	for (i = 0; i < ss->_used; ++i) {
		Statement* s = &ss->_statements[i]; 
		//printf("Statement line number: %d\n", s->_line_num);
		printf("%d%s%f\n", s->_line_num, " ", get_suspiciousness((s->_percentage_failed), s->_percentage_passed));
	}
}

void print_suspicious_statements(Statements* ss) {
	int i = 0;
	//printf("------------------------------------------------------------------------\n");
	//printf("Printing suspicious statements.\n");
	for (i = 0; i < ss->_used; ++i) {
		Statement* s = &ss->_statements[i]; 
		double suspiciousness = get_suspiciousness((s->_percentage_failed), s->_percentage_passed);
		if (suspiciousness >= 0.50) {
			//printf("%d%s%f\n", s->_line_num, "  suspiciousness: ", suspiciousness);
			printf("%d%s%f\n", s->_line_num, " ", suspiciousness);
		}
		
	}
}