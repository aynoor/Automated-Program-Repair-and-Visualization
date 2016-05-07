#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/DebugInfo.h"
#include "llvm/IR/Metadata.h"
#include "llvm/IR/Module.h"
#include "llvm/IRReader/IRReader.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/SourceMgr.h"
#include "llvm/IR/InstIterator.h"
#include "llvm/IR/CallSite.h"
#include "llvm/Analysis/CallGraph.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/InstrTypes.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"
#include "llvm/IR/Function.h"
#include <iostream>
#include <string>
#include <sstream>
#include <cstring>
#include <fstream>
using namespace llvm;
using namespace std;


int main(int argc, char **argv) {
  if (argc < 2) {
    errs() << "Usage: " << argv[0] << " <IR file>\n";
    return 1;
  }

  // Parse the input LLVM IR file into a module.
  SMDiagnostic Err;
  std::unique_ptr<Module> m = parseIRFile(argv[1], Err, getGlobalContext());
  if (!m) {
    Err.print(argv[0], errs());
    return 1;
  }

  // ... use module

  string line; 
  ifstream faulty_line_file("faulty_line_num.txt");
  unsigned faulty_line_num;
  if (faulty_line_file.is_open()) {
    while (getline(faulty_line_file, line)) {
      istringstream ss(line);
      ss >> faulty_line_num;
    }
    bool line_found = false;
    for (auto& F : m->getFunctionList()) {
      for (auto &B : F) {
        for (auto &I : B) {
          unsigned current_line_num;
          if (DILocation *Loc = I.getDebugLoc()) {          // Here I is an LLVM instruction
            unsigned Line = Loc->getLine();
            StringRef File = Loc->getFilename();
            StringRef Dir = Loc->getDirectory();
            if (File.find("TC") != string::npos) {}
            else {
              if (current_line_num != Line) {
                current_line_num = Line;
                if (Line == faulty_line_num) {
                  errs() << F.getName() << " " << File << " " << Dir << "\n";
                  string func_name;
                  istringstream ss(F.getName());
                  ss >> func_name;
                  ofstream faulty_function_file ("faulty_function.txt");
                  if (faulty_function_file.is_open()) {
                    faulty_function_file << func_name << "\n";
                    faulty_function_file.close();
                  }
                  else cout << "Unable to open file";
                  line_found = true;
                  break;
                }
              }
            }
          }
        }
        if (line_found) break;
      }
      if (line_found) break;
    } 
  } else {
    cout << "Unable to open file faulty_line_num.txt";
  }


}
 