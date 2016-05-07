var sys = require('sys');
var exec = require('child_process').exec;
var child;
var lineReader = require('line-reader');
child = exec("g++ Program_Repair.cpp", function (error, stdout, stderr) {
	sys.print('stdout: ' + stdout);
	var exe = exec("./a.out '/Files/ClientFiles/Sourc'", function (err, out, derr) {
		sys.print('stdout: * ' + out);
	});
});

