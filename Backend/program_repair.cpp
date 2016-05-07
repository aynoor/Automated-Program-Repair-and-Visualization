#include <iostream>
#include <cstdlib>
#include <fstream>
#include <dirent.h>
#include <sstream>
#include <string>
#include <map>
#include <vector>
#include <algorithm>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <fcntl.h>
using namespace std;

int get_suspicious_line(map<int, double> suspiciousness) {
  int faulty_line;
  double max = -1.0;
  for (map<int, double>::iterator it = suspiciousness.begin(); it != suspiciousness.end(); ++it) {
    if (it->second > max) {
      max = it->second;
      faulty_line = it->first;
    }
  }
  return faulty_line;
}

void print_map(map<int, double> _map) {
  for(map<int, double>::iterator it = _map.begin(); it != _map.end(); ++it) {
    cout << it->first << " => " << it->second << endl; 
  }
}

void create_terminate_program_file() {
  ofstream terminate_program;
  terminate_program.open("terminate_program.txt");
  terminate_program << "false";
  terminate_program.close();
}

map<int, double> read_suspicousness_file(const char* path) {
  string line;
  bool begin_making = false;
  map<int, double> suspicious_stmnts;
    ifstream suspiciousness_file(path);
    if (suspiciousness_file.is_open()) {
      while ( getline (suspiciousness_file,line) ) {
        if (begin_making) {
          istringstream ss(line);
          int line_num;
          double suspiciousness;
          ss >> line_num >> suspiciousness;
          suspicious_stmnts[line_num] = suspiciousness;
        }
        if (line == "Most suspicious statements: ") begin_making = true;
      }
      suspiciousness_file.close();
    }
    return suspicious_stmnts;
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

bool all_testcases_passed() {
  string line;
  bool passed = true;
    ifstream results_file("results.txt");
    if (results_file.is_open()) {
      while ( getline (results_file,line) ) {
        istringstream ss(line);
        int line_num;
        string status;
        ss >> line_num >> status;
        if (status == "F") {
          passed = false;
          break;
        }
      }
      results_file.close();
    }
    return passed;
}

bool if_termimate_program() {
  string line;
  ifstream terminate_program_file("terminate_program.txt");
  getline (terminate_program_file,line);
  if (line == "true") {
    return true;
  }
  return false;
}

int write_integer(const char* filename, int to_write) {
  ofstream faulty_line_file;
  faulty_line_file.open(filename);
  faulty_line_file << to_write;
  faulty_line_file.close();
}

int write_string(const char* filename, const char* to_write) {
  ofstream faulty_line_file;
  faulty_line_file.open(filename);
  faulty_line_file << to_write;
  faulty_line_file.close();
}

void create_new_file(const char* filename) {
  ofstream new_file;
  remove(filename);
  new_file.open(filename);
  new_file.close();
}

string get_variable_name() {
  string var_name;
  string reading_var_name;
  ifstream replace_with_var_file("replace_with_var.txt");

  while (getline (replace_with_var_file, reading_var_name)) {
    var_name = reading_var_name;
  }
  istringstream iss(var_name);
  vector<std::string> string_parts;
  string part;
  while (std::getline(iss, part, '.')) {
    if (!part.empty())
        string_parts.push_back(part);
  }
  return string_parts[0];
}

void create_var_name_file() {
  ofstream var_file ("if_replaced.txt");
  if (var_file.is_open())
  {
    var_file << "false" << "\n";
    var_file.close();
  }
  else cout << "Unable to open file replace with var.";
}

// file only has one line
string read_file(string filename) {
  ifstream fix_file (filename);
  string line = "";
  if (fix_file.is_open()) {
    getline(fix_file, line);
  }
  return line;
}

void modify_file(string fix_type) {
  create_var_name_file();
  string line;
  int num_of_tc; 
  vector<int> testcase_numbers;
  ifstream num_tc ("num_testcase_files.txt");
  bool read_total_tc_num = false;
  if (num_tc.is_open()) {
    while ( getline (num_tc,line) ) {
      if (read_total_tc_num) {
        istringstream ss(line);
        ss >> num_of_tc;
        break;
      }
      if (line == "Total TCs: ") {
        read_total_tc_num = true;
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
  //cout <<"total num_of_tc: " << num_of_tc << endl;
  //cout << "size of vector: " << testcase_numbers.size() << endl;
  //for (int i = 0; i < testcase_numbers.size(); ++i) cout << "testcase #: " << testcase_numbers[i] << endl;
  for (int i = 0; i < testcase_numbers.size(); ++i) {
    //cout << testcase_numbers[i] << endl;
    string testcase_name = "TC" + to_string(testcase_numbers[i]) + ".bc";
    string testcase_name_m = "TC" + to_string(testcase_numbers[i]) + "_m.bc";
    //cout << "Modified tc name: " << testcase_name_m << endl;
    if (fix_type == "vr") {
      system(("opt -load /home/llvm/Desktop/compliedllvm/lib/LLVMHello.so -replace_values < " + testcase_name+ " > " + 
      testcase_name_m).c_str()); 
    } else if (fix_type == "oc") {
      //cout << "IN FIX OC" << endl;
      system(("opt -load /home/llvm/Desktop/compliedllvm/lib/LLVMHello.so -operator_change < " + testcase_name + " > " + 
      testcase_name_m).c_str()); 
    }
    //cout << testcase_name_m << endl;
    //execl(("lli " + testcase_name_m + " > one_result.txt").c_str(), "", (char *)0);
    //system(("lli " + testcase_name_m + " > one_result.txt").c_str());
    int pid = fork();
    //cout << "pid: " << pid << endl;
    if (pid == 0) {
    // CHILD PROCESS
      //system(("lli " + testcase_name_m + " > one_result.txt").c_str());
    int fd = open("one_result.txt", O_CREAT | O_WRONLY);    // 
    dup2(fd,1);
    //cout << testcase_name_m << endl;
    execl("/usr/local/bin/lli", "/usr/local/bin/lli", testcase_name_m.c_str());

    //  execl(("lli " + testcase_name_m + " > one_result.txt").c_str(), "", (char *)0);
    } else if (pid < 0) {
      // Forking failed
    } else {
      string tc_name = to_string(testcase_numbers[i]);
      //cout << testcase_name_m << endl;
      sleep(3);
      kill(pid, SIGKILL);


      system("chmod 666 one_result.txt");
      ifstream num_tc ("one_result.txt");
      if (num_tc.is_open()) {
        int counter = 0;
        ofstream results("results.txt", ios_base::out | ios_base::app);
        bool written = false;
        while ( getline (num_tc,line) ) {
          written = true;
          if (counter == 0) results << line <<" "; //cout << line << endl;
          if (counter == 1) results << line << "\n"; //cout << line << endl;
          counter++;
        }
        if (written == false) {
          ofstream results("results.txt", ios_base::out | ios_base::app);
          results << tc_name << " ";
          results << "F" << "\n";
          num_tc.close();
        }
        num_tc.close();
      }
    }
  }
}

int main() {
    //system("opt -load /home/llvm/Desktop/compliedllvm/lib/LLVMHello.so -replace_values < testcase_median.bc > testcase_median_m.bc"); // name of the programm you want to start
    // Create file to terminate program
    

    // Create replace_with_var file
    create_new_file("replace_with_var.txt");

    
    
    // Get Faulty Line number
    map<int, double> suspicious_stmnts = read_suspicousness_file("suspiciousness.txt");
    make_line_colors_file("suspiciousness.txt");
    if (suspicious_stmnts.empty()) {
      write_string("faulty_line_num.txt", "no faulty line present/found.");
      cout << "no likely repair needed/found" << endl;
    } else {
      bool possibly_repaired = false;
      int faulty_line_num;
      string tc_results = "testcase_results";
      int lines_tried = 0;
      string fix = "";
      while (!suspicious_stmnts.empty()) {
        ++lines_tried;
        create_terminate_program_file();
        faulty_line_num = get_suspicious_line(suspicious_stmnts);
        //faulty_line_num = 8;
        suspicious_stmnts.erase(faulty_line_num);
        string tc_file_name = tc_results +".txt"; //+ to_string(lines_tried) + 
        create_new_file(tc_file_name.c_str());
        int iterations = 0;
        // write fauty line number file
        write_integer("faulty_line_num.txt", faulty_line_num);
        
        // get fix type
        string source_bitcode_name = "source_bitcode.bc";
        system(("opt -load /home/llvm/Desktop/compliedllvm/lib/LLVMHello.so -inst_type < " + source_bitcode_name + " > " + 
        "/dev/null").c_str()); 
        fix = read_file("fix_type.txt");
        //fix = "vr";
       
        remove("one_result.txt");   // will give an error if it doesn't exist but nbd

        if (fix != "") {
           // Transform code and run test cases on it
          if (fix == "vr") {
            size_t index = 0;                        // index of value to get from value symbol table
            while(true) {
              create_new_file("results.txt");
              // write index file
              write_integer("index.txt", index);
              ++index;
              ++iterations;
              // Run the passcd Dco
              modify_file(fix);
              
              ifstream results_file("results.txt");
              ofstream testcase_results_file(tc_file_name, ios_base::out | ios_base::app);
              //ofstream _results("results.txt", ios_base::out | ios_base::app);
              testcase_results_file << iterations << "\n";
              for (string file_line; getline(results_file, file_line); )
              {
                  testcase_results_file << file_line << "\n";
              }

              if (all_testcases_passed()) {
                possibly_repaired = true; 
                break;
              } 
              if (if_termimate_program()){
                break;
              }
            }
          } else if (fix == "oc") {
            //cout << "FL #: " << faulty_line_num << endl;
            string current_opcode = read_file("current_opcode.txt");
            vector<string> opcodes;
            opcodes.push_back("mul"); opcodes.push_back("add"); opcodes.push_back("sub"); opcodes.push_back("sdiv"); 
            //cout << "possibly repaired: " << possibly_repaired << "\n";
            for (int i = 0; i < opcodes.size(); ++i) {
              if (current_opcode != opcodes[i]) {
                write_integer("index.txt", i);
                write_string("try_opcode.txt", opcodes[i].c_str());
                create_new_file("results.txt");
                ++iterations;
                //cout << "iterations: " << iterations << endl;
                //cout << "===========================================================" << endl;
                modify_file(fix);
                //cout << "===========================================================" << endl;
                ifstream results_file("results.txt");
                ofstream testcase_results_file(tc_file_name, ios_base::out | ios_base::app);
                //ofstream _results("results.txt", ios_base::out | ios_base::app);
                testcase_results_file << iterations << "\n";
                for (string file_line; getline(results_file, file_line); )
                {
                    testcase_results_file << file_line << "\n";
                }
                if (all_testcases_passed() ) {    //|| opcodes[i] == "add"
                  possibly_repaired = true; 
                  //cout << "possibly repaired: *************" << possibly_repaired << "\n";
                  break;
                } 
              }
            }
          }
        }
        //cout << "lines tried: " << lines_tried << "\n";
        
        if (possibly_repaired) break;
      }
    
    // Proposed fix
    if (fix != "") {
      if (fix == "vr") {
        if (possibly_repaired) {
          cout << "Replace the value of variable at line number " << faulty_line_num << " with " << get_variable_name() << "."<< endl;
        } else {
          cout << "No likely repair found." << endl;
        }
      } else if (fix == "oc") {
        if (possibly_repaired) {
          // not checking if read file contains ""
          cout << "Replace the operator at line number " << faulty_line_num << " with " << read_file("try_opcode.txt") << "."<< endl;
        } else {
          cout << "No likely repair found." << endl;
        }
      }
    }
    
  }
    // Delete files
    /*if (remove("faulty_line_num.txt")  != 0) cout << "Error deleting file faulty_line_num.txt.";
    if (remove("index.txt")  != 0) cout << "Error deleting file index.txt.";  
    if (remove("results.txt")  != 0) cout << "Error deleting file results.txt.";  
    if (remove("terminate_program.txt")  != 0) cout << "Error deleting file terminate_program.txt.";  
    if (remove("replace_with_var.txt")  != 0) cout << "Error deleting file replace_with_var.txt.";  */
    return 0;
}

