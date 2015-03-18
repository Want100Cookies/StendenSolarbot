var express = require("express");
var app = express();

var wPort = 3700;

app.get("/", function(req, res) {
	res.sendfile("./public/index.html");
});

var io = require("socket.io").listen(app.listen(wPort));
console.log("Listening on port " + wPort);


io.sockets.on('connection', function(socket) {
	socket.emit('message', 'Hi there');

	socket.on('ping', function(data) {
		socket.emit('message', 'pong');
		console.log("Socket " + socket.id + " pinged the server.");
	});

	socket.on('initComPorts', function(data) {
		initComPorts();
	});

	socket.on('listActive', function(data) {
		console.log(sPorts);
	})
});

var serialport = require('serialport');
var SerialPort = serialport.SerialPort;
var readData;

var sPorts = new Array();

function initComPorts() {
	serialport.list( function(err, ports) {
		ports.forEach( function(port) {
			sPorts.push({
				comPort: new SerialPort(port.comName, { baudrate:9600, parser: serialport.parsers.readline("\n") }, false),
				robotName: "",
				game: ""});
			console.log(port.comName + " is available.");
		});
		for(var i = 0; i < sPorts.length; i++) {
			console.log("Trying to set up comport " + sPorts[i].comPort.path);
			setupPortHandler(sPorts[i], i);
		}
	});
}

function setupPortHandler(robotObject, i) {
	var comPort = robotObject.comPort;
	comPort.on("error", function(err) {
		console.log(err);
	});

	comPort.open(function(err) {
		if (err) {
			console.log(err);
			sPorts.splice(i, 1);
			return;
		} else {
			console.log(comPort.path + " opened");
		}

		comPort.on("data", function(data){
			io.sockets.emit('message', data);
		});
	});
}

// sp.on('open', function() {
// 	console.log('Serial port opened');
// 	io.sockets.emit('message', 'Serial port opened');
// 	sp.on('data', function(data) {
// 		readData += data.toString();
// 		if (readData.indexOf('{') >= 0 && readData.indexOf('}') >= 0) {
// 			cleanData = "{" + readData.substring(readData.indexOf('{') + 1, readData.indexOf('}')) + "}";
// 		    readData = '';
// 		    io.sockets.emit('message', cleanData);
// 		}
// 	})
// });
