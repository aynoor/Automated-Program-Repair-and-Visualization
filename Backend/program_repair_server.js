// Connect to server
var io = require('socket.io/node_modules/socket.io-client');
var socket = io.connect('http://172.16.201.1:8000', {reconnect: true});
socket.on('connect', function(socket) {
	console.log('Connected to Web Server.');
	send_msg_to_server("I'm backend server.");
	var rtlib_file_path = "FaultLocalization/rtlib.c";
	var rtlib_bitcode_name = "rtlib.bc";
	var rtlib_bitcode = exec("clang -O0 -g -emit-llvm " + rtlib_file_path  +  " -c -o " + rtlib_bitcode_name, function (_error, _stdout, _stderr) {
      if (_error !== null) console.log('compilation error: ' + _error);
    });
});
function send_msg_to_server(msg) {
	socket.emit('server_llvm', msg);
}

/*var ping = require ("net-ping");

var session = ping.createSession ();

session.pingHost (socket, function (error, target) {
    if (error)
        console.log (target + ": " + error.toString ());
    else
        console.log (target + ": Alive");
});*/

setInterval(function() {
	/*console.log("1");
	var webserver_connected = false;
	console.log(socket.connected);
	if (socket.connected) {
		console.log("20");
	    socket.on('alive', function() {
			console.log("webserver response received");
			webserver_connected = true;
		});
	}
	console.log("hi");
	if (webserver_connected == false) {
		console.log("webserver dead");
	}*/
	if (socket != null) {
		if (socket.connected) {
			console.log("Webserver alive");
		} else {

			console.log("webserver dead");
			process.exit(1);
		}
	}
}, 5000);


// send ping to webserver
/*var heartbeats = require('heartbeats');
var heart = heartbeats.createHeart('1000');

heart.createEvent(7, function (heartbeat, last) {
	var webserver_connected = false;
	socket.emit('alive');
	console.log("message emitted");
	
	console.log("1");
	if (webserver_connected) {
		console.log("Webserver alive.");
	} else {
		console.log("Webserver dead.");
	}
	console.log("2");
});*/

// USER MANAGEMENT

var fs = require('fs');

var directory_path = "Files/ClientFiles/";
var uploaded_source_code_files_path = "Files/SourceCodeFiles/";
var uploaded_test_case_files_path = "Files/TestCasesFiles/";

var clients = [];
var backend_socket;
var request_client_socket;
var mkdirp = require('mkdirp');

function make_new_directory(_path) {
  mkdirp(_path, function (error) {
    if (error) {
      console.log("The directory " + _path + " could not be made due to the following error: \n" ); 
      console.log(error);
    }
  });
}

function add_client (id) {
  client_map = {};
  client_map['client_id'] = id;                          // needed to get directory name and perhaps it might be useful in future
  client_map['dir_path'] = directory_path + id + "/";
  make_new_directory(client_map['dir_path']);
  client_map['source_code_path'] = client_map['dir_path'] + "SourceCodeFiles/";
  make_new_directory(client_map['source_code_path']);
  client_map['testcase_path'] = client_map['dir_path'] + "TestCasesFiles/";
  make_new_directory(client_map['testcase_path']);
  clients.push(client_map);
}

function find_client_by_id(id) {
  var id_of_client = -1;
  clients.forEach( function loop (client, index) {
    if (loop.stop) return;
    if (id == client.client_id) {
      id_of_client = client.client_id;
      loop.stop = true;
    }
  });
  return id_of_client;
}


var client_not_found = "Client not found.";

function find_client_file_path (id, source_or_testcase) {
  var path_found = false;
  var _path = "";
  clients.forEach( function loop(client, index) {
  	console.log(client.client_id + " " + id);
    if (loop.stop) return;
    if (id == client.client_id) {
      if (source_or_testcase == "source") {         // make sure it's source in the code below too
        _path = client.source_code_path; 
        path_found = true; 
        loop.stop = true;
      } else if (source_or_testcase == "testcase") {
        _path = client.testcase_path;
        path_found = true; 
        loop.stop = true;
      }                   
    }
  });
  if (path_found) return _path;
  else return "Client not found.";                                                            
}

function if_client_present(id) {
	var if_present = false;
	clients.forEach( function loop (client, index) {
	    if (loop.stop) return;
	    if (id == client.client_id) {
	      if_present = true;
	      loop.stop = true;
	    }
	});
	return if_present;
}

socket.on('source_code_file', function (file, file_name, client_id) {
	console.log('Source code file received: ' + file_name);
	if (!if_client_present(client_id)) {
		console.log("adding client");
		add_client(client_id);
	}
    var _path = find_client_file_path(client_id, "source"); 
    if (_path != client_not_found) {          // no need to check for null
        fs.writeFile(_path + file_name, file, function(error) {
            if (error)  console.log('Source code file could not be saved, error: ' + error);
            else  {
              console.log('Source code file saved.');
              var source_bitcode_name = "source_bitcode.bc";
              var make_bitcode = exec("clang -O0 -g -emit-llvm " + _path + file_name + " -c -o " + source_bitcode_name, function (error, stdout, stderr) {
              	if (error !== null) console.log('compilation error: ' + error);
              	var line_num_file = exec("./FaultLocalization/line_num " + source_bitcode_name + " > line_numbers.txt", function (_error, _stdout, _stderr) {
              		if (_error !== null) console.log('compilation error: ' + _error);
              	});
              });
            }
        });
    } else {
        console.log("Client no longer connected.");
    } 
});

socket.on('remove_tc_trace', function() {
	fs.unlink("testcase_trace.txt");
});


socket.on('testcase_file', function (file, file_name, client_id) {
	console.log('Test case file received: ' + file_name);
	if (!if_client_present(client_id)) {
		add_client(client_id);
	}
    var _path = find_client_file_path(client_id, "testcase"); 
    if (_path != client_not_found) {          // no need to check for null
        fs.writeFile(_path + file_name, file, function(error) {
            if (error)  console.log('Testcase file could not be saved, error: ' + error);
            else  {
              console.log('Testcase file saved.');
            }
        });
    } else {
        console.log("Client no longer connected.");
    } 
});





var sys = require('sys');
var exec = require('child_process').exec;
var child;
var lineReader = require('line-reader');
socket.on('send_data', function (client_id) {
	child = exec("g++ -std=c++0x  Program_Repair.cpp", function (error, stdout, stderr) {
		var source_code_path = find_client_file_path(client_id, "source");
		var testcase_file_path = find_client_file_path(client_id, "testcase");
		console.log("client id: " + client_id);
		console.log("source code path: "  + source_code_path);
		console.log("testcase path: " + testcase_file_path);
	
		if (error !== null) console.log('exec error: ' + error);

		var exe = exec("./a.out " + source_code_path + " " + testcase_file_path, function (err, out, derr) {
			if (err !== null) console.log('exec err: ' + error);
			fs.readFile('faulty_line_num.txt', 'utf8', function (error, line_num) {
				if (error) {
					console.log(error);
				} else {
					if (line_num > 0) {
						socket.emit('faulty_line_num', line_num); 	
					} else {
						socket.emit('faulty_line_num', "No faulty line found/present"); 	
					}
				}
			});

			fs.readFile('suggested_fix.txt', 'utf8', function (error, sugg_fix) {
				if (error) console.log(error);
				else socket.emit('suggested_fix', sugg_fix);
			});

			var iterations = 1;
			testcase_results_with_fixes = {};
			results_data = {};
			lineReader.eachLine('testcase_results.txt', function(line, last_line) {
				console.log(line);
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
				    	if (line != _iterations && _iterations <= Object.keys(testcase_results_with_fixes).length) {
				    		var tc_results = testcase_results_with_fixes[_iterations];
				    		console.log(_iterations);
				    		console.log(line+"-----------------");
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
			
			// send line colors 
			var line_colors = {};
			var splitted_line;
			lineReader.eachLine('line_colors.txt', function(line, last_line) {
				splitted_line = line.split(" ");
				line_colors[splitted_line[0]] = splitted_line[1];
				if (last_line) {
					splitted_line = line.split(" ");
					line_colors[splitted_line[0]] = splitted_line[1];
					console.log(JSON.stringify(line_colors));
					socket.emit('Line_Colors', JSON.stringify(line_colors));
				}
			});

		});
	});
});

socket.on('send_converage', function (client_id, testcase_name) {
	child = exec("g++ -std=c++0x tc_cov_res.cpp -o tc", function (error, stdout, stderr) {
		var source_code_path = find_client_file_path(client_id, "source");
		var testcase_file_path = find_client_file_path(client_id, "testcase");
		console.log("client id: " + client_id);
		console.log("source code path: "  + source_code_path);
		console.log("testcase path: " + testcase_file_path);
	
		if (error !== null) console.log('compilation error: ' + error);
		console.log("./tc " + " " + testcase_file_path + " " + testcase_name );
		var exe = exec("./tc " + " " + testcase_file_path + " " + testcase_name , function (err, out, derr) {
			if (err !== null) console.log('executable err: ' + err);

			// send line colors 
			var line_colors = {};
			var splitted_line;
			lineReader.eachLine('line_colors.txt', function(line, last_line) {
				splitted_line = line.split(" ");
				line_colors[splitted_line[0]] = splitted_line[1];
				if (last_line) {
					splitted_line = line.split(" ");
					line_colors[splitted_line[0]] = splitted_line[1];
					//console.log(JSON.stringify(line_colors));
					socket.emit('Line_Colors', JSON.stringify(line_colors));
					var cov_tc_result = {}; var testcase_num; var counter = 0; var tc_result;
					lineReader.eachLine('testcase_result_cov.txt', function( first_line, end_line) {
						console.log(first_line);
						if (counter == 0)  testcase_num  = first_line;
						else tc_result = first_line;
						++counter;
						if (end_line) {
							cov_tc_result[testcase_num] = tc_result;
							console.log(JSON.stringify(cov_tc_result));
							socket.emit('tc_cov_result', JSON.stringify(cov_tc_result));
						}
					});
				}
			});

		});
	});
});



// To print the results map
/*
[results].forEach(function(obj) {
	Object.keys(obj).forEach( function(key) {
		var node =obj[key];
		console.log(node);
		fs.appendFile(test_nodes, JSON.stringify(node) + "\r\n" , function (error) {
			if(error) {
				console.log(error);
			} 
		});
	});
});
*/