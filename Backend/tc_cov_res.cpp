#include <iostream>
#include <cstdlib>
#include <fstream>
#include <dirent.h>
#include <sstream>
#include <string>
#include <map>
#include <vector>
#include <string>
#include <algorithm>
#include <sstream>

using namespace std;


void create_new_file(const char* filename) {
  ofstream new_file;
  remove(filename);
  new_file.open(filename);
  new_file.close();
}


bool check(string testcase_names, string num) {
	string testcase_number = testcase_names.substr(2,1);
	if (testcase_number.compare(num) == 0) {
		return true;
	}
	return false;
}


void generate_testcase_output_file(string dir_path, string testcase_names) {
	DIR *directory;
	struct dirent *dir;
	directory = opendir(dir_path.c_str());
	string file_name = "";
	int num_of_files = 0;
	create_new_file("num_testcase_files.txt");
	ofstream num_tc("num_testcase_files.txt", ios_base::out | ios_base::app);
	if (directory) {
		while ((dir = readdir(directory)) != NULL) {
			// check if it's really a file and not (sym)link, directory, etc
			// DT_REG a regular file
			if (dir->d_type == DT_REG) 	{
				file_name = dir->d_name;
				string path_to_file = dir_path + file_name;
				size_t dot_found = file_name.find(".");
				string testcase_number = file_name.substr(2,1);
				num_tc << testcase_number << "\n";
				if (check(testcase_names, testcase_number)) {
					++num_of_files;
					string file_bitcode = file_name.substr(0, dot_found+1) + "bc";
					// create bitcode file
					system(("clang -O0 -g -emit-llvm " + path_to_file + " -c -o " + file_bitcode).c_str());
					// run tracc pass
					string modified_bc_file = file_bitcode.substr(0, dot_found) + "_t.bc";
					//cout << modified_bc_file << endl;
					system(("opt -load /home/llvm/Desktop/compliedllvm/lib/LLVMHello.so -dynamic_trace < " + file_bitcode +
					" > " + modified_bc_file).c_str());
					// link the to rtlib file
					system(("llvm-link " + modified_bc_file + " rtlib.bc -o linked.bc").c_str());
					// save testcase trace
					system(("lli linked.bc >> testcase_trace.txt"));	
				}
			}
		}
		closedir(directory);
	}
	//cout << to_string(num_of_files) << endl;
	
	num_tc << "Total TCs: \n";
	num_tc << to_string(num_of_files) << "\n";
	num_tc.close();
}



void make_line_colors_file(const char* path) {
  ofstream line_colors_file;
  line_colors_file.open("line_colors.txt");
  
  string line;
  bool making = false;
  ifstream suspiciousness_file(path);
  while (getline(suspiciousness_file, line)) {
    if (line == "Most suspicious statements: ") making = false;
    if (making) {
      line_colors_file << line << "\n";
    } 
    if (line == "Color of Statements: ") making = true;
  }
  line_colors_file.close();
}


int main(int argc, char * inputs[]) {
	string path = inputs[1];
	string tc_name = inputs[2];
	cout << path << "	" << tc_name << endl;
	// vector<string> testcase_names;
	// testcase_names.push_back("TC1");
	// //testcase_names.push_back("TC6");
	// testcase_names.push_back("TC2");
	generate_testcase_output_file(path, tc_name);
	remove("suspiciousness1.txt");
	// Run fault_localization technique

	system("gcc FaultLocalization/fault_localization.c -o fl");
	system("./fl > suspiciousness1.txt");


	remove("line_colors.txt");
    make_line_colors_file("suspiciousness1.txt");

	// delete testcase_trace.txt file otherwise when the program is run again data will be appended to it
	//if (remove("testcase_trace.txt")  != 0) cout << "Error deleting file index.txt."; 
    //cout << "lli " + tc_name.substr(0,3) + ".bc"  << endl;
    string testcase_bitcode = tc_name.substr(0,3) + ".bc";
    ifstream infile(testcase_bitcode);
    if (!infile.good()){
    	system(("clang -O0 -g -emit-llvm " + path + tc_name + " -c -o " + testcase_bitcode).c_str());
    } 
    	
    system(("lli " + testcase_bitcode + " > testcase_result_cov.txt").c_str());
    
	return 0;
}