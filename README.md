# Automated-Program-Repair-and-Visualization
Developing an automatic program repair tool using state of the art research algorithms in fault localization and program repair to detect erroneous lines and suggest fixes. Developed heuristics based on patterns of common programming mistakes. The tool provides a thorough visualization of fault localization and program repair to better understand how the technique works and to be more effective for the programmer. Used custom C++ passes in LLVM compiler infrastructure, a C++ backend control engine, and a NodeJS frontend server.

How to run:

Need to install LLVM and compile the passes provided in AutomaticProgramRepair/LLVMCustomPasses/Hello.cpp

1) Go to AutomaticProgramRepair/Frontend_and_Client/App and run the following command in the terminal:
    nodejs app.js
    
2) Got to AutomaticProgramRepair/Backend and run thr following command in another terminal window or tab:
    nodejs program_repair_server.js
    
3) To connect a client go to browser and type
    http://0.0.0.0:8000/


