#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/InstrTypes.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/DebugInfo.h"
#include "llvm/IR/Metadata.h"
#include "llvm/IRReader/IRReader.h"
#include "llvm/Support/SourceMgr.h"
#include "llvm/IR/InstIterator.h"
#include "llvm/IR/CallSite.h"
#include "llvm/Analysis/CallGraph.h"
#include "llvm/IR/ValueSymbolTable.h"
#include <iostream>
#include <string>
#include <map>
#include <fstream>
#include <sstream>
using namespace llvm;
using namespace std;


namespace {
  struct FunctionName : public FunctionPass {
    static char ID;
    FunctionName() : FunctionPass(ID) {}

    bool runOnFunction(Function &F) override {
      errs().write_escaped(F.getName()) << '\n';
      return false;
    }
  };
}

char FunctionName::ID = 0;
static RegisterPass<FunctionName> Y("function_name", "This pass prints the names of all the functions.", false, false);

namespace {
  struct StaticTrace : public FunctionPass {
    static char ID;
    StaticTrace() : FunctionPass(ID) {}

    bool runOnFunction(Function &F) override {
      LLVMContext &Ctx = F.getContext();
      Constant *logFunc = F.getParent()->getOrInsertFunction(
        "logop", Type::getVoidTy(Ctx), Type::getInt32Ty(Ctx), NULL
      );
      
      for (auto &B : F) {
        for (auto &I : B) {
          //if (F.getName() == "grade") {
            unsigned current_line_num;
            if (DILocation *Loc = I.getDebugLoc()) { // Here I is an LLVM instruction
              unsigned Line = Loc->getLine();
              //StringRef File = Loc->getFilename();
              //StringRef Dir = Loc->getDirectory();
              if (current_line_num != Line) {
                errs() << "Line number: " << Line << "\n";
                Instruction *inst = &I;
                IRBuilder<> builder(inst);
                ConstantInt *a = builder.getInt32(Line);
                builder.CreateCall(logFunc, a);
                current_line_num = Line;
              }
            }
          //}
        }
      }
      return false;
    }
  };
}

char StaticTrace::ID = 0;
static RegisterPass<StaticTrace> B("static_trace", "Statically trace execution of the program.", false, false);


namespace {
  struct Trace : public FunctionPass {
    static char ID;
    Trace() : FunctionPass(ID) {}

    bool runOnFunction(Function &F) override {
      LLVMContext &Ctx = F.getContext();
      Constant *logFunc = F.getParent()->getOrInsertFunction(
        "logop", Type::getVoidTy(Ctx), Type::getInt32Ty(Ctx), NULL
      );
      for (auto &B : F) {
        int c = 0;
        for (auto &I : B) {
          if (c == 0) {
            if (DILocation *Loc = I.getDebugLoc()) { // Here I is an LLVM instruction
              unsigned Line = Loc->getLine();
              //StringRef File = Loc->getFilename();
              //StringRef Dir = Loc->getDirectory();
              errs() << "Start of basic block line number: " << Line << "\n";
              ++c;
              Instruction *inst = &I;
              IRBuilder<> builder(inst);
              //Value *strPtr = builder.CreateGlobalStringPtr("/dev/mem", ".str");
              ConstantInt *a = builder.getInt32(Line);
              //builder.CreateCall2(logop,strPtr,a);
              builder.CreateCall(logFunc, a);
            }
          }

        }
      }
      return false;
    }
  };
}

char Trace::ID = 0;
static RegisterPass<Trace> Z("trace", "Trace execution of the program.", false, false);

string get_file_path() {
  string line; 
  ifstream file_path("/home/llvm/Desktop/Sproj/file_path.txt");
  if (file_path.is_open()) {
    while (getline(file_path, line)) {
      return line;
    }
  } else {
    return "";
  }
}

string get_func_name() {
  string file_path = get_file_path();
  if (file_path != "") {
    string line;
    ifstream faulty_func(file_path+"faulty_function.txt");
    if (faulty_func.is_open()) {
      while(getline(faulty_func, line)) {
        faulty_func.close();
        return line;
      }
    } else {
      return "";
    }
  }
}

namespace {
  struct DynamicTrace : public FunctionPass {
    static char ID;
    DynamicTrace() : FunctionPass(ID) {}

    bool runOnFunction(Function &F) override {
      LLVMContext &Ctx = F.getContext();
      Constant *logFunc = F.getParent()->getOrInsertFunction(
        "logop", Type::getVoidTy(Ctx), Type::getInt32Ty(Ctx), NULL
      );

      for (auto &B : F) {
        //errs().write_escaped(F.getName()) << '\n';
        //errs() << F.getName() << "\n";
        int c = 0;
        for (auto &I : B) {
          //if (c == 0 ) {      // && (F.getName() != "main")
            /* c == 0 is added because in IR form instructions get broken down like m=y; so line numbers are printed 
                again and again so, to get it only one time this condition is used
            */
            unsigned current_line_num;
            if (DILocation *Loc = I.getDebugLoc()) { // Here I is an LLVM instruction
              unsigned Line = Loc->getLine();
              StringRef File = Loc->getFilename();
              if (File.find("TC") != string::npos) {}
              else {
                if (current_line_num != Line) {
                  current_line_num = Line;
                  //StringRef Dir = Loc->getDirectory();
                  //errs() << "Start of basic block line number: " << Line << "\n";
                  ++c;
                  Instruction *inst = &I;
                  IRBuilder<> builder(inst);
                  //Value *strPtr = builder.CreateGlobalStringPtr("/dev/mem", ".str");
                  ConstantInt *a = builder.getInt32(Line);
                  //builder.CreateCall2(logop,strPtr,a);
                  builder.CreateCall(logFunc, a);
                }
              }
            }
          //}

        }
      }
      return false;
    }
  };
}

char DynamicTrace::ID = 0;
static RegisterPass<DynamicTrace> A("dynamic_trace", "Dynamic trace of a program..", false, false);


bool if_replaced() {
  string line;
  ifstream if_replaced_file("if_replaced.txt");
  getline (if_replaced_file,line);
  if (line == "false") {
    return true;
  }
  return false;
}

Type* get_faulty_inst_type(Function &F) {
  string read_line_num;
  ifstream faulty_line_file(get_file_path()+"faulty_line_num.txt");
  if (faulty_line_file.is_open()) {
          /*while ( getline (myfile,read_line_num) ) {
            errs() << read_line_num << '\n';
          }*/
    getline (faulty_line_file,read_line_num);
    faulty_line_file.close();
  }
  unsigned int faulty_line_num = atoi(read_line_num.c_str());

  // find and return instruction type
  errs() << F.getName() << "\n";
  for (auto &B : F) {
    for (auto &I : B) {
      if (DILocation *Loc = I.getDebugLoc()) { // Here I is an LLVM instruction
        unsigned Line = Loc->getLine();
        StringRef File = Loc->getFilename();
        //StringRef Dir = Loc->getDirectory();
        if (File.find("TC") != string::npos) {}
        else {
          errs() << Line << "\n";
          Value* v_temp = &I;
          errs() <<"Name: " << v_temp->getName() << " value: " << v_temp << "  Instruction type: " << I.getType()->getTypeID() << "\n";
          if (faulty_line_num == Line) {
            Value* v_temp = &I;
            errs() << v_temp <<  " Faulty Instruction type: " << I.getType()->getTypeID() << "\n";

            return I.getType();
            /*LoadInst* AI = dyn_cast<LoadInst>(&I);
            if (AI) {
              return AI->getType(); 
            }*/
              /*AllocaInst* AI = dyn_cast<AllocaInst>(&I);
              if (AI) {
                return AI->getAllocatedType();
              }*/
          }
        }
      }
    }
  }
  return NULL;
}

namespace {
  struct Replace : public FunctionPass {
    static char ID;
    Replace() : FunctionPass(ID) {}
    Value* replace_with = NULL;

    bool runOnFunction(Function &F) override {
      ValueSymbolTable& sym_table = F.getValueSymbolTable();
      vector<Value*> values_to_try;
      for (auto iter1 = sym_table.begin(); iter1 != sym_table.end(); ++iter1) {
        StringMapEntry<Value*> &map_entry = *iter1;
        if (F.getName() != "main") {
          AllocaInst* AI = dyn_cast<AllocaInst>((map_entry.getValue()));
          if (AI) {
            if ((AI->getAllocatedType()->getTypeID()) == 11) {
              //errs() << "Nme " << map_entry.getValue() << "\n";
              values_to_try.push_back(map_entry.getValue());
            }
          }
        }
      }
      if (F.getName() != "main") {
        // read faulty line num
        string read_line;
        ifstream faulty_line_file(get_file_path()+"faulty_line_num.txt");
        if (faulty_line_file.is_open()) {
          /*while ( getline (myfile,read_line) ) {
            errs() << read_line << '\n';
          }*/
          getline (faulty_line_file,read_line);
          faulty_line_file.close();
        }
        unsigned int faulty_line_num = atoi(read_line.c_str());
        // read index of value to try
        string read_index;
        ifstream index_file(get_file_path()+"index.txt");
        if (index_file.is_open()) {
          /*while ( getline (myfile,read_line) ) {
            errs() << read_line << '\n';
          }*/
          getline (index_file,read_index);
          index_file.close();
        }
        unsigned int value_index = atoi(read_index.c_str());
        if (value_index < values_to_try.size()) {
          bool replaced = false;
          for (auto &B : F) {
            for (auto &I : B) {
              if (DILocation *Loc = I.getDebugLoc()) { // Here I is an LLVM instruction
                unsigned Line = Loc->getLine();
                StringRef File = Loc->getFilename();
                //StringRef Dir = Loc->getDirectory();
                if (File.find("TC") != string::npos) {}
                else {
                  //errs() << Line << "\n";
                  Value* v_temp = &I;
                  //errs() <<"Name: " << v_temp->getName() << "  Instruction type: " << I.getType()->getTypeID() << "\n";
                  //errs() << "Compareing values: " << v_temp << " " << values_to_try[value_index] << "\n";
                  if (faulty_line_num == Line) {
                    //errs() << "Faulty Instruction type: " << I.getType()->getTypeID() << "\n";
                    LoadInst* LI = dyn_cast<LoadInst>(&I);

                    if (LI && v_temp != values_to_try[value_index]) {
                      //errs() << "Line " << Line << "\n"; 
                      if (LI != values_to_try[value_index]) {
                        ReplaceInstWithInst(LI, new LoadInst(values_to_try[value_index], ""));
                        replaced = true;
                        //errs() << "File name: " << File << " function name: " << F.getName() << "\n";
                        if (if_replaced()) {
                          //errs() << values_to_try[value_index]->getName() << " " << values_to_try[value_index]->getValueName() << "\n";
                          ofstream replace_with_var_file;
                          replace_with_var_file.open("replace_with_var.txt", ios_base::app);
                          replace_with_var_file << (value_index+1) << "\n";
                          replace_with_var_file << (string) values_to_try[value_index]->getName() + "\n";

                          ofstream if_replaced_file;
                          if_replaced_file.open("if_replaced.txt");
                          if_replaced_file << "true";
                          if_replaced_file.close();

                          //errs() << if_replace() << "\n";
                          //errs() << value_index << "\n";
                          //errs() << (string) values_to_try[value_index]->getName() + "\n";
                          /*ofstream replace_with_var;
                          replace_with_var.open("replace_with_var.txt");
                          replace_with_var << (string) values_to_try[value_index]->getName();*/
                          replace_with_var_file.close();
                        }
                        break;
                      } 
                    }
                  }
                }
              }
            }
            if (replaced) break;
          }
        } else {
          ofstream terminate_program;
          terminate_program.open("terminate_program.txt");
          terminate_program << "true";
          terminate_program.close();
        }
      }
      return false;
    }
  };
}

char Replace::ID = 0;
static RegisterPass<Replace> C("replace_values", "Replace values of one variable wth all preceding variable values.", false, false);


namespace {
  struct VariableReplacement : public FunctionPass {
    static char ID;
    VariableReplacement() : FunctionPass(ID) {}
    
    bool runOnFunction(Function &F) override {
      Type* inst_type = get_faulty_inst_type(F);
      //if (inst_type) {
        errs() << "Fautly instruction type: " << inst_type->getTypeID() << "\n";
        ValueSymbolTable& sym_table = F.getValueSymbolTable();
      vector<Value*> values_to_try;
      for (auto iter1 = sym_table.begin(); iter1 != sym_table.end(); ++iter1) {
        StringMapEntry<Value*> &map_entry = *iter1;
        if (F.getName() != "main") {
          AllocaInst* AI = dyn_cast<AllocaInst>((map_entry.getValue()));
          if (AI) {
            if ((AI->getAllocatedType()->getTypeID()) == 3) {
              errs() << "Nme " << map_entry.getValue() << "\n";
              values_to_try.push_back(map_entry.getValue());
            }
          }
        }
      }
      bool terminate = false;
      if (F.getName() != "main") {
        // read faulty line num
        string read_line;
        ifstream faulty_line_file(get_file_path()+"faulty_line_num.txt");
        errs() << "File path; " <<  get_file_path() << "\n";
        errs() << "HIIIIIIIIIIIIIIIIIII" << "\n";
        if (faulty_line_file.is_open()) {
          /*while ( getline (myfile,read_line) ) {
            errs() << read_line << '\n';
          }*/
          getline (faulty_line_file,read_line);
          faulty_line_file.close();
        }
        unsigned int faulty_line_num = atoi(read_line.c_str());
        // read index of value to try
        string read_index;
        ifstream index_file(get_file_path()+"index.txt");
        if (index_file.is_open()) {
          /*while ( getline (myfile,read_line) ) {
            errs() << read_line << '\n';
          }*/
          getline (index_file,read_index);
          index_file.close();
        }
        unsigned int value_index = atoi(read_index.c_str());
        if (value_index < values_to_try.size()) {
          bool replaced = false;
          for (auto &B : F) {
            for (auto &I : B) {
              if (DILocation *Loc = I.getDebugLoc()) { // Here I is an LLVM instruction
                unsigned Line = Loc->getLine();
                StringRef File = Loc->getFilename();
                //StringRef Dir = Loc->getDirectory();
                if (File.find("TC") != string::npos) {}
                else {
                  errs() << Line << "\n";
                  Value* v_temp = &I;
                  errs() <<"Name: " << v_temp->getName() << "  Instruction type: " << I.getType()->getTypeID() << "\n";
                  errs() << "Compareing values: " << v_temp << " " << values_to_try[value_index] << "\n";
                  if (faulty_line_num == Line) {
                    errs() << "Faulty Instruction type: " << I.getType()->getTypeID() << "\n";
                    LoadInst* LI = dyn_cast<LoadInst>(&I);

                    if (LI && v_temp != values_to_try[value_index]) {
                      //errs() << "Line 9\n"; 
                      if (LI != values_to_try[value_index]) {
                        ReplaceInstWithInst(LI, new LoadInst(values_to_try[value_index], ""));
                        replaced = true;
                        //errs() << "File name: " << File << " function name: " << F.getName() << "\n";
                        if (if_replaced()) {
                          //errs() << values_to_try[value_index]->getName() << " " << values_to_try[value_index]->getValueName() << "\n";
                          ofstream replace_with_var_file;
                          replace_with_var_file.open("replace_with_var.txt", ios_base::app);
                          replace_with_var_file << (value_index+1) << "\n";
                          replace_with_var_file << (string) values_to_try[value_index]->getName() + "\n";

                          ofstream if_replaced_file;
                          if_replaced_file.open("if_replaced.txt");
                          if_replaced_file << "true";
                          if_replaced_file.close();

                          //errs() << if_replace() << "\n";
                          //errs() << value_index << "\n";
                          //errs() << (string) values_to_try[value_index]->getName() + "\n";
                          /*ofstream replace_with_var;
                          replace_with_var.open("replace_with_var.txt");
                          replace_with_var << (string) values_to_try[value_index]->getName();*/
                          replace_with_var_file.close();
                        }
                        break;
                      } 
                    }
                  }
                }
              }
            }
            if (replaced) break;
          }
        } else {
          ofstream terminate_program;
          terminate_program.open("terminate_program.txt");
          terminate_program << "true";
          terminate_program.close();
          terminate = true;
        }
      }
      errs() << "terminate: " << terminate << "\n";
      //if(terminate) return false;
      /*} else {
        errs() << "**************Instruction type returned NULL***************" << "\n";
      }*/

      return false;
    }
  };
}

char VariableReplacement::ID = 0;
static RegisterPass<VariableReplacement> E("Variable_Replacement", "Replace values of one variable wth all preceding variable values.", false, false);


namespace {
  struct GetInstType : public FunctionPass {
    static char ID;
    GetInstType() : FunctionPass(ID) {}
    vector<Value*> values_to_try;
    map<char*, Type*> values;
    bool runOnFunction(Function &F) override {
      errs() << "Function name: " << F.getName() << "\n";
      ValueSymbolTable& sym_table = F.getValueSymbolTable();
      if (!sym_table.empty()) {
        //errs() << "Size of symbol table: " << sym_table.size() << "\n";
      }
      //if (F.getName() == "main") {
      for (auto iter1 = sym_table.begin(); iter1 != sym_table.end(); ++iter1) {
        StringMapEntry<Value*> &map_entry = *iter1;
        //errs() << "=> Key: " << map_entry.getKey() << "\n";
        //errs() << "Key data: " << map_entry.getKeyData() << "\n";
        //errs() << "    Get value: " << *(map_entry.getValue()) << "\n";
        AllocaInst* AI = dyn_cast<AllocaInst>((map_entry.getValue()));
          if (AI) {
            //if ((AI->getAllocatedType()->getTypeID()) == 11) {
              values_to_try.push_back(map_entry.getValue());
              values[(char*) map_entry.getKey().data()] = AI->getAllocatedType();                 // char* key = (char*) map_entry.getKey().data();
            //}
            //errs() << " Instruction type: " << *(AI->getAllocatedType())  << " \n"; 
            //errs() << " Instruction type ID: " << (AI->getAllocatedType()->getTypeID())  << " \n";                 
          }
      //}
      
      //errs() << "No. of values found: " << values_to_try.size() << " in map: " << values.size() << "\n";
      for (map<char*,Type*>::iterator iter = values.begin(); iter != values.end(); ++iter) {
        if (iter->second->isFloatTy())
          errs() << iter->first << " => " << iter->second->getTypeID() << '\n';
      }
      /*for (auto &B : F) {
        for (auto &I : B) {
          if (DILocation *Loc = I.getDebugLoc()) { // Here I is an LLVM instruction
            unsigned Line = Loc->getLine();
            //errs() << "Line num: " << Line << "\n";
            AllocaInst* AI = dyn_cast<AllocaInst>(&I);
            if (AI) {
              errs() << "Line number: " << Line << " Instruction type: " << AI->getAllocatedType()->getTypeID()  << " \n";                 
              }
          }
        }
      }*/
      }
      return false;
    }
  };
}

char GetInstType::ID = 0;
static RegisterPass<GetInstType> F("get_inst_type", "Get type of instructions in the function/module.", false, false);


// Fix no. 2, Operator change

namespace {
  struct OperatorChange : public FunctionPass {
    static char ID;
    OperatorChange() : FunctionPass(ID) {}
    bool runOnFunction(Function &F) override {
      //errs().write_escaped(F.getName()) << '\n';

      if (F.getName() != "main") {
        // read faulty line num
        string read_line;
        ifstream faulty_line_file(get_file_path()+"faulty_line_num.txt");
        if (faulty_line_file.is_open()) {
          getline (faulty_line_file,read_line);
          faulty_line_file.close();
        }
        unsigned int faulty_line_num = atoi(read_line.c_str()); 
        // read index of value to try
        string read_index;
        ifstream index_file(get_file_path()+"index.txt");
        if (index_file.is_open()) {
          getline (index_file,read_index);
          index_file.close();
        }
        unsigned int value_index = atoi(read_index.c_str());
        bool replaced = false;
        for (auto& B : F) {
          for (auto& I : B) {
            if (DILocation *Loc = I.getDebugLoc()) { // Here I is an LLVM instruction
              unsigned Line = Loc->getLine();
              StringRef File = Loc->getFilename();
              //StringRef Dir = Loc->getDirectory();
              if (File.find("TC") != string::npos) {}
              else {
                //change my sequence
                if (faulty_line_num == Line) {
                  if (auto* op = dyn_cast<BinaryOperator>(&I)) {
                    string opcode_to_try;
                    ifstream try_opcode (get_file_path()+"try_opcode.txt");
                    if (try_opcode.is_open()) {
                      getline (try_opcode,opcode_to_try);
                      try_opcode.close();
                    }
                    errs() << "opcode to try: " << opcode_to_try << "\n";
                    //errs() << "Opcode name: " <<  op->getOpcodeName() << "\n";
                    //errs() << "Opcode: " <<  op->getOpcode() << "\n";
                    // Insert at the point where the instruction `op` appears.
                    IRBuilder<> builder(op);
                    Value* new_opcode;
                    if (opcode_to_try == "sub" || opcode_to_try == "add" || opcode_to_try == "mul" || opcode_to_try == "sdiv") {
                      if (opcode_to_try == "sub") {
                        // Make a multiply with the same operands as `op`.
                        Value* lhs = op->getOperand(0);
                        Value* rhs = op->getOperand(1);
                        new_opcode = builder.CreateSub(lhs, rhs);
                        errs() << "tried sub" << "\n";
                      } else if (opcode_to_try == "add") {
                        // Make a multiply with the same operands as `op`.
                        Value* lhs = op->getOperand(0);
                        Value* rhs = op->getOperand(1);
                        new_opcode = builder.CreateAdd(lhs, rhs);
                        errs() << "tried add" << "\n";
                      } else if (opcode_to_try == "mul") {
                        // Make a multiply with the same operands as `op`.
                        Value* lhs = op->getOperand(0);
                        Value* rhs = op->getOperand(1);
                        new_opcode = builder.CreateMul(lhs, rhs);
                        errs() << "tried mul" << "\n";
                      } else if (opcode_to_try == "sdiv") {
                        // Make a multiply with the same operands as `op`.
                        Value* lhs = op->getOperand(0);
                        Value* rhs = op->getOperand(1);
                        new_opcode = builder.CreateSDiv(lhs, rhs);
                        errs() << "tried sdiv" << "\n";
                      }
                      // Everywhere the old instruction was used as an operand, use our
                      // new multiply instruction instead.
                      for (auto& U : op->uses()) {
                        User* user = U.getUser();  // A User is anything with operands.
                        user->setOperand(U.getOperandNo(), new_opcode);
                      }
                      replaced = true;
                      if (if_replaced()) {
                        //errs() << values_to_try[value_index]->getName() << " " << values_to_try[value_index]->getValueName() << "\n";
                        ofstream replace_with_var_file;
                        replace_with_var_file.open("replace_with_var.txt", ios_base::app);
                        replace_with_var_file << (value_index+1) << "\n";
                        replace_with_var_file << opcode_to_try+ "\n";
                        ofstream if_replaced_file;
                        if_replaced_file.open("if_replaced.txt");
                        if_replaced_file << "true";
                        if_replaced_file.close();
                        replace_with_var_file.close();
                      }
                      // We modified the code.
                      return true;
                    } else {
                      errs() << "Incorrect opcode to try." << "\n";
                    }
                  }
                }
              }
            }
          }
        }
      }
      return false;
    }
  };
}

char OperatorChange::ID = 0;
static RegisterPass<OperatorChange> D("operator_change", "This changes the operators for a given instruction.", false, false);

int write_string(string filename, const char* to_write) {
  ofstream faulty_line_file;
  faulty_line_file.open(filename);
  faulty_line_file << to_write;
  faulty_line_file.close();
}

namespace {
  struct InstType : public FunctionPass {
    static char ID;
    InstType() : FunctionPass(ID) {}

    bool runOnFunction(Function &F) override {
      //errs().write_escaped(F.getName()) << '\n';
      // read faulty line num
      string read_line;
      ifstream faulty_line_file(get_file_path()+"faulty_line_num.txt");
      if (faulty_line_file.is_open()) {
        getline (faulty_line_file,read_line);
        faulty_line_file.close();
      }
      unsigned int faulty_line_num = atoi(read_line.c_str()); 
      for (auto &B : F) {
        for (auto &I : B) {
          if (DILocation *Loc = I.getDebugLoc()) { // Here I is an LLVM instruction
            unsigned Line = Loc->getLine();
            StringRef File = Loc->getFilename();
            //StringRef Dir = Loc->getDirectory();
            if (File.find("TC") != string::npos) {}
            else {
              if (faulty_line_num == Line) {
                // if load instruction than Variable Replacement fix
                LoadInst* LI = dyn_cast<LoadInst>(&I);
                // if Binary operator present than Operator Chaneg fix
                auto* op = dyn_cast<BinaryOperator>(&I);
                if (LI) {
                  write_string(get_file_path()+"fix_type.txt", "vr");
                } else if (op) {
                  write_string(get_file_path()+"fix_type.txt", "oc");
                  write_string(get_file_path()+"current_opcode.txt", op->getOpcodeName());
                }
              }
            }
          }
        }
      }
      return false;
    }
  };
}

char InstType::ID = 0;
static RegisterPass<InstType> G("inst_type", "This pass prints the instruction type of the statement at a given line number.", false, false);


namespace {
  struct MissingAssignment : public FunctionPass {
    static char ID;
    MissingAssignment() : FunctionPass(ID) {}

    bool runOnFunction(Function &F) override {
      //errs().write_escaped(F.getName()) << '\n';
      // read faulty line num
      string read_line;
      /*ifstream faulty_line_file(get_file_path()+"faulty_line_num.txt");
      if (faulty_line_file.is_open()) {
        getline (faulty_line_file,read_line);
        faulty_line_file.close();
      }*/
      LLVMContext &Ctx = F.getContext();
      //unsigned int faulty_line_num = atoi(read_line.c_str()); 
      unsigned int faulty_line_num = 3;
      for (auto &B : F) {
        for (auto &I : B) {
          if (DILocation *Loc = I.getDebugLoc()) { // Here I is an LLVM instruction
            unsigned Line = Loc->getLine();
            StringRef File = Loc->getFilename();
            //StringRef Dir = Loc->getDirectory();
            if (File.find("TC") != string::npos) {}
            else {
              if (faulty_line_num == Line) {
                // Add a allocation statement initializing the variable at faulty line
                Instruction *current_inst = &I;
                BasicBlock *bb = &B;
                AllocaInst *new_inst = new AllocaInst(Type::getInt32Ty(Ctx), 0, "declared");
                bb->getInstList().insert(current_inst, new_inst);
                // code modified
                return  true;
              }
            }
          }
        }
      }
      return false;
    }
  };
}

char MissingAssignment::ID = 0;
static RegisterPass<MissingAssignment> H("missing_assignment", "This pass initializes ariable that might not have been assigned.", false, false);
