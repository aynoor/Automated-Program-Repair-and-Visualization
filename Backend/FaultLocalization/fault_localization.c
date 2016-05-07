#include <stdio.h>
#include "test_suite.c"
#include "Statements.c"

int main(int argc, const char** argv) {

	// Read all the line numbers of the function from the input file line_numbers.txt
	FILE* line_nums_file = fopen ("line_numbers.txt", "r");
	Statements stmnts;
	initialize_statements(&stmnts, 1);
	char buffer[1024];
	Array line_numbers;
	initialize_array(&line_numbers, 1);  // initially 5 elements
	while (fgets(buffer, 1024, line_nums_file) != NULL) { 
		Statement s;
		int line_num; 
		sscanf(buffer, "%d", &line_num);
		initialize_statement (&s, line_num); 
		insert_statement(&stmnts, s);
	}
	
	fclose (line_nums_file); 

	// Include all the test case results in the test suite
	test_suite ts;
	size_t  size_of_test_suite = 1;
	initialize_test_suite(&ts, size_of_test_suite);

	FILE* input_file = fopen ("testcase_trace.txt", "r");
  	char buff[1024];
  	size_t size_of_test_case_BB = 1;							//size of start line nums of BBs
  	while (fgets(buff, 1024, input_file) != NULL) {  
  		test_case tc;
  		int id;
  		bool lines_read = false;
  		sscanf(buff, "%d", &id);
  		initialize_test_case (&tc, id, size_of_test_case_BB);
    	while (!lines_read) {
    		if (fgets(buff, 1024, input_file) != NULL){
    			if (strcmp(buff, "P\n") != 0 && (strcmp(buff, "F\n")) != 0 ) {
    				int line_num;
					sscanf(buff, "%d", &line_num);
					insert_BB_line_num(&tc, line_num);
				} else {
					lines_read = true;
				}
			}
		}
		size_t len = strlen(buff) - 1;								// remove null char after Pass or Fail
		if (buff[len] == '\n')
    		buff[len] = '\0';

		set_test_case_status(&tc, *buff);
		insert_test_suite(&ts, tc);
		
    }

	fclose (input_file);      
	//print_test_suite(&ts);
	// Find total number of passed and failed test cases
	num_of_failed_test_cases(&ts);
	num_of_passed_test_cases(&ts);

	// Find percentage passed and failed for every statement
	int statement_index = 0;
	for (statement_index = 0; statement_index < stmnts._used; ++statement_index) {
		set_percentage_passed(&stmnts._statements[statement_index], percentage_passed_s(&ts, stmnts._statements[statement_index]._line_num));
		set_percentage_failed(&stmnts._statements[statement_index], percentage_failed_s(&ts, stmnts._statements[statement_index]._line_num));
	}

	//print_statements(&stmnts);
	// Find and print suspicious statements

	print_color_range(&stmnts);
	printf("%s\n", "Most suspicious statements: ");
	print_suspicious_statements(&stmnts); 
	return 0;
}