const sql = require("mysql2");
const path= require('path');
const file = require('fs');
const confPath= path.join(__dirname, '/configuration.json');

var connection;

//Create a connection to a database.
function createDbConnection() {

	//Read the configuration file.
	try {
		var data= JSON.parse(file.readFileSync(confPath));

		connection= sql.createConnection({
			host: data.dbConf.host,
			user: data.dbConf.user,
			password: data.dbConf.password,
			database: data.dbConf.database
		});
		
	} catch (error) {
		return console.error('Error: ' + error.message);
	}

	connection.connect(function(error) {
  		if (error) {
    		return console.error('error: ' + error.message);
  		}
  		//console.log('Connected to the MySQL server.');
	});
}

//Shows sensor data.
function showSensorData(callback) {
	var query;

	createDbConnection();
	query= `select * from device where device_id= 1`;
	connection.query(query,function(err, result, fields) {
		  //console.log(results); // Results contains rows returned by server.
		  //console.log(fields); // Fields contains extra meta data about results, if available.
		if(err){
			console.log(err.message);
			connection.end();
			return callback("failure");

		}else{
			connection.end();
			return callback(result);
		}
	});
}

//Toggle the test LED.
function toggleTestLed(request, callback) {
	var query;
	var values;

	query= `update device set topic_test_led= ? where device_id= ?`;
	values= [request.testLed, 1];

	createDbConnection();

	connection.query(query, values, (err, result)=>{
		if(err){
			console.log(err.message);
			connection.end();
			return callback("failure");
		}else{
			//console.log("Record updated with ID: " + JSON.stringify(result));
			connection.end();
			return callback("success");
		}
	});
}

//Updates the lamp brightness.
function changeLampBrightness(request, callback) {
	var query;
	var values;

	query= `update device set topic_led_strip= ? where device_id= ?`;
	values= [request.lampBrightness, 1];

	createDbConnection();

	connection.query(query, values, (err, result)=>{
		if(err){
			console.log(err.message);
			connection.end();
			return callback("failure");
		}else{
			//console.log("Record updated with ID: " + JSON.stringify(result));
			connection.end();
			return callback("success");
		}
	});
}

//Updates the water pump speed.
function changeWaterPumpSpeed(request, callback) {
	var query;
	var values;

	query= `update device set topic_water_pump= ? where device_id= ?`;
	values= [request.waterPumpSpeed, 1];

	createDbConnection();

	connection.query(query, values, (err, result)=>{
		if(err){
			console.log(err.message);
			connection.end();
			return callback("failure");
		}else{
			//console.log("Record updated with ID: " + JSON.stringify(result));
			connection.end();
			return callback("success");
		}
	});
}



//Exporting class members to the public.
module.exports.showSensorData= showSensorData;
module.exports.toggleTestLed= toggleTestLed;
module.exports.changeLampBrightness= changeLampBrightness;
module.exports.changeWaterPumpSpeed= changeWaterPumpSpeed;

