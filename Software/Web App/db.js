const sql = require("mysql2");

var connection;

//Create a connection to a database.
function createDbConnection() {

	//Read the configuration file.
	try {
		var data= JSON.parse(fs.readFileSync('./configuration.json'));

		//console.log(data.dbConf.host);

		connection = sql.createConnection({
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

//Handles the user sign up process.
function signUp(request, callback) {
	var query;
	var values;

	//console.log(request.email + request.firstName + request.age + request.password);
	createDbConnection();
	query= `insert into user(email, first_name, age, password) values (?, ?, ?, ?)`;
	values= [request.email, request.firstName, request.age, request.password];

	connection.query(query, values, (err, result)=>{
		if(err){
			connection.end();
			return callback("failure");
		}else{
			//console.log("Record inserted with ID: " + JSON.stringify(result));
			connection.end();
			return callback("success");
		}
	});
}

//Updates customer data.
function editProfile(request, userId, password, changePassword, callback) {
	var query;
	var values;

	//console.log('Change password: ' + changePassword);
	//console.log('Password: ' + password);

	if(changePassword== 'false'){
		query= `update user set email= ?, first_name= ?, last_name= ?, street= ?, city= ? where user_id= ?`;
		values= [request.email, request.firstName, request.lastName, request.street, request.city, userId];

	}else if(changePassword== 'true'){
		password= bcrypt.hashSync(password, saltRounds);
		//console.log('Password hash: ' + password);

		query= `update user set email= ?, first_name= ?, last_name= ?, street= ?, city= ?, password= ? where user_id= ?`
		values= [request.email, request.firstName, request.lastName, request.street, request.city, password, userId];
	}

	//console.log(request.email + request.firstName + request.age + request.password);
	createDbConnection();
	//query= `update user set email= ?, first_name= ?, last_name= ?, street= ?, city= ?, password= ? where user_id= ?`;
	//values= [request.email, request.firstName, request.lastName, request.street, request.city, password, userId];

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

//Handles listing all the users.
function showSensorData(callback) {
	var query;

	createDbConnection();
	query= `select * from device`;
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

//Exporting class members to the public.
module.exports.signUp= signUp;
module.exports.showSensorData= showSensorData;
