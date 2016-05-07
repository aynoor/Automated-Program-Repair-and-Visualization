
var sys = require('sys');
var exec = require('child_process').exec;
var child;
var lineReader = require('line-reader');

			var iterations = 1;
			testcase_results_with_fixes = {};
			results_data = {};
			lineReader.eachLine('testcase_results.txt', function(line, last_line) {
				if (line == iterations) {
					if (iterations > 1) {
						testcase_results_with_fixes[iterations-1] = results_data;
						results_data = {};
						iterations++;
					} else {
						iterations++;
					}
				} else {
					var splitted_line = line.split(" ");
					results_data[splitted_line[0]] = splitted_line[1];
				}
				if (last_line) {
					var splitted_line = line.split(" ");
					results_data[splitted_line[0]] = splitted_line[1];
					testcase_results_with_fixes[iterations-1] = results_data;
					
					console.log(JSON.stringify(testcase_results_with_fixes));
					var _iterations = 1;
					var prev_iter = 0;
				    lineReader.eachLine('replace_with_var.txt', function (line, last_line) {
				    	if (line != _iterations) {
				    		var tc_results = testcase_results_with_fixes[_iterations];
				    		console.log(_iterations);
				    		console.log(line);
				    		tc_results["fix"] = line;
				    		testcase_results_with_fixes[_iterations] = tc_results;
				    		_iterations++;
				    	}
				    	if (last_line) {
				    		console.log(JSON.stringify(testcase_results_with_fixes));
				    		socket.emit('tc_results_with_fixes', JSON.stringify(testcase_results_with_fixes));
				    	}
				    });
				}
		    });