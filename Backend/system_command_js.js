var sys = require('sys');
var exec = require('child_process').exec;
var child;
child = exec("g++ -std=c++0x  Program_Repair.cpp", function (error, stdout, stderr) {
	sys.print('stdout: ' + stdout);
	var exe = exec("./a.out 'Files/ClientFiles/10/SourceCodeFiles/' 'Files/ClientFiles/10/TestCasesFiles/'", function (err, out, derr) {
		sys.print('stdout: * ' + out);
	});
});

