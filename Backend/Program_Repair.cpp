#include <iostream>
#include <cstdlib>
#include <fstream>
#include <dirent.h>
#include <sstream>
#include <map>
#include <vector>
#include <string>
#include <algorithm>
#include <sstream>
using namespace std;

string read_source_code_file(string file_path) {
	DIR *directory;
	struct dirent *dir;
	directory = opendir(file_path.c_str());
	string file_name = "";
	if (directory) {
		while ((dir = readdir(directory)) != NULL) {
			// check if it's really a file and not (sym)link, directory, etc
			// DT_REG a regular file
			if (dir->d_type == DT_REG) 	{
				file_name = dir->d_name;
			}
		}
		closedir(directory);
	}
	return file_name;
}

void create_new_file(const char* filename) {
  ofstream new_file;
  remove(filename);
  new_file.open(filename);
  new_file.close();
}

void generate_testcase_output_file(string dir_path) {
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
				++num_of_files;
				file_name = dir->d_name;
				string path_to_file = dir_path + file_name;
				size_t dot_found = file_name.find(".");
				string testcase_number = file_name.substr(2,1);
				num_tc << testcase_number << "\n";
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
		closedir(directory);
	}
	//cout << to_string(num_of_files) << endl;
	
	num_tc << "Total TCs: \n";
	num_tc << to_string(num_of_files) << "\n";
	num_tc.close();
}


int main(int argc, char * client_files_paths[]) {
	string sourcecode_file_path = client_files_paths[1];
	string testcase_dir_path = client_files_paths[2];
	
	string sourcecode_file_name = read_source_code_file(sourcecode_file_path);
	if (sourcecode_file_name != "") {
		string source_bitcode_name = "source_bitcode.bc";
		string full_file_path = sourcecode_file_path+sourcecode_file_name;
		// Create a bitcode file of source code
		// This being made by NodeJS backend server, remove it from here
		system(("clang -O0 -g -emit-llvm " + full_file_path +  " -c -o " + source_bitcode_name).c_str());
		/*int ret;
		if (!(ret=system(("clang -O0 -g -emit-llvm " + full_file_path +  " -c -o " + source_bitcode_name).c_str())))  {
			cout << "executed" << endl;	
		}  else {
			cout << "failed!" << endl;
		}*/

		// Get static trace of source code file 
		system(("./FaultLocalization/line_num " + source_bitcode_name + " > line_numbers.txt").c_str());

		// Get the dynamic trace of all the testcases
		string rtlib_file_path = "FaultLocalization/rtlib.c";
		string rtlib_bitcode_name = "rtlib.bc";
		system(("clang -O0 -g -emit-llvm " + rtlib_file_path  +  " -c -o " + rtlib_bitcode_name).c_str());

		generate_testcase_output_file(testcase_dir_path);

		// Run fault_localization technique
		system("gcc FaultLocalization/fault_localization.c -o fl");
		system("./fl > suspiciousness.txt");

		// delete testcase_trace.txt file otherwise when the program is run again data will be appended to it
		if (remove("testcase_trace.txt")  != 0) cout << "Error deleting file index.txt.";  

		// Suugest fix 
		system("g++ -std=c++0x program_repair.cpp -o pr");
		system("./pr > suggested_fix.txt");

	} else {
	}

}
