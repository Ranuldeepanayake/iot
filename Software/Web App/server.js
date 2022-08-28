//Includes.
const express = require('express');
const bodyParser = require("body-parser");
const path= require('path');
const file = require('fs');

const database = require('./db.js');

//Global constants.
const app = express();
const port = 3000;

app.use(bodyParser.urlencoded({extended: true}));
//app.use(express.json());
app.use(express.static(__dirname + '/ejs/'));	//Use Ranga's static resources.
app.set('view engine', 'ejs')

//Path preparation.
const ejsPath= path.join(__dirname, '/ejs/');

app.all('/', function(req, res){
		database.showSensorData(function (result){
			console.log("**************Showing sensor data>");
			console.log(result);
			res.render(ejsPath + 'index.ejs', { test: 'success', sensorData: result});
		});

});

app.post('/test', function(req, res){
    console.log("**************Toggling the test LED>");
    console.log(req.body);

    database.toggleTestLed(req.body, function(result){
        //Add error handling code if the result is a failure.
        //console.log(result);

        database.showSensorData(function (result){
            //Add error handling code if the result is a failure.
            //console.log(result);
            res.render(ejsPath + 'index.ejs', { test: 'success', sensorData: result});
        });
    });
});

app.post('/lamp', function(req, res){
    console.log("**************Changing the lamp brightness>");
    console.log(req.body);

    database.changeLampBrightness(req.body, function(result){
        //Add error handling code if the result is a failure.
        //console.log(result);

        database.showSensorData(function (result){
            //Add error handling code if the result is a failure.
            //console.log(result);
            res.render(ejsPath + 'index.ejs', { test: 'success', sensorData: result});
        });
    });
});

app.post('/pump', function(req, res){
    console.log("**************Changing the water pump speed>");
    console.log(req.body);

    database.changeWaterPumpSpeed(req.body, function(result){
        //Add error handling code if the result is a failure.
        //console.log(result);

        database.showSensorData(function (result){
            //Add error handling code if the result is a failure.
            //console.log(result);
            res.render(ejsPath + 'index.ejs', { test: 'success', sensorData: result});
        });
    });
});

//Handle invalid URLs.
app.all('*', function(req, res) {
    res.send('Bad request');
});

//Starts a nodejs server instance.
app.listen(port, () => {
    console.log(`Example app listening at http://localhost:${port}`);
  });