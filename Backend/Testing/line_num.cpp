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
#include <cstring>
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

  for (auto& F : m->getFunctionList()) {
    bool printed = false;
    for (auto &B : F) {
        for (auto &I : B) {
          unsigned current_line_num;
          if (DILocation *Loc = I.getDebugLoc()) {          // Here I is an LLVM instruction
            unsigned Line = Loc->getLine();
            if (printed == false) {
              //StringRef File = Loc->getFilename();
              //StringRef Dir = Loc->getDirectory();
              errs() << Loc->getFilename() << "\n";
              errs() << Loc->getDirectory() << "\n";
              errs() << F.getName() << "\n"; 
              printed = true;
            }
            if (current_line_num != Line) {
              cout << Line << endl;
              current_line_num = Line;
            }
          }
        }
      }
    }  
  }
 