#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

vector<int> read_testcase_num () {
  string line;
  vector<int> testcase_numbers;
  ifstream num_tc ("num_testcase_files.txt");
  bool read_total_tc_num = false;
  if (num_tc.is_open()) {
    while ( getline (num_tc,line) ) {
      if (line == "Total TCs: ") {
        break;
      }
      if (read_total_tc_num == false) {
        int number;
        istringstream ss(line);
        ss >> number;
        testcase_numbers.push_back(number);
      }
    }
    num_tc.close();
  }
  return testcase_numbers;
}


int main() {
	// Delete files, Error will occur when a file doesn't exist;
	remove("./a.out");

    remove("faulty_line_num.txt"); 				//cout << "Error deleting file faulty_line_num.txt.";
    remove("fix_type.txt");
    remove("./fl");
    remove("./pr");
    remove("if_replaced.txt");
    remove("line_colors.txt");
    remove("line_numbers.txt");
    remove("linked.bc");
    remove("index.txt");						//cout << "Error deleting file index.txt.";  
    remove("one_result.txt"); 					//cout << "Error deleting file one_result.txt.";  
    remove("results.txt"); 						//cout << "Error deleting file results.txt.";

    remove("source_bitcode.bc");  				//cout << "Error deleting file source_bitcode.bc.";  
    remove("rtlib.bc"); 						//cout << "Error deleting file rtlib.bc.";  

    remove("replace_with_var.txt"); 			//cout << "Error deleting file replace_with_var.txt."; 
    remove("suggested_fix.txt");				//cout << "Error deleting file suggested_fix.txt.";  
    remove("suspiciousness.txt"); 				//cout << "Error deleting file suspiciousness.txt.";

    vector<int> testcase_numbers = read_testcase_num();
    for (int i = 0; i < testcase_numbers.size(); ++i) {
    	string tc_name = "TC" + to_string(testcase_numbers[i]) + ".bc";
    	string tc_trace_name = "TC" + to_string(testcase_numbers[i]) + "_t.bc";
    	string tc_modified_name = "TC" + to_string(testcase_numbers[i]) + "_m.bc";
    	remove(tc_name.c_str());				//cout << "Error deleting file " + tc_name;
    	remove(tc_trace_name.c_str());			//cout << "Error deleting file " + tc_trace_name;
    	remove(tc_modified_name.c_str());		//cout << "Error deleting file " + tc_modified_name;
    }
    
    remove("num_testcase_files.txt");  

    remove("terminate_program.txt");		//cout << "Error deleting file terminate_program.txt.";  
    remove("testcase_results.txt");			//cout << "Error deleting file testcase_results.txt."; 
    
    remove("current_opcode.txt"); 	
    remove("try_opcode.txt"); 			//cout << "Error deleting file try_opcode.txt , it probably doesn't exist."; 

    remove("suspiciousness1.txt");
    remove("testcase_trace.txt");
    remove("testcase_result_cov.txt");
    
	return 0;
}