//Includes.
const express = require('express');
const bodyParser = require("body-parser");
const path= require('path');
const file = require('fs');

const customer = require('./db.js');

//Global constants.
const app = express();
const port = 3000;

app.use(bodyParser.urlencoded({extended: true}));
app.use(express.static(__dirname + '/agriSmart/'));	//Use Ranga's static resources.
app.set('view engine', 'ejs')

//Path preparation.
const ejsPath= path.join(__dirname, '/ejs/');

app.get('/', function(req, res){
		db.showSensorData(function (result){
			console.log("**************Showing sensor data>");
			console.log(result);
			res.render(ejsPath + 'index.ejs', { test: 'success', sensorData: result});
		});

});

app.get('/lamp', function(req, res){
    db.showSensorData(function (result){
        console.log("**************Showing sensor data>");
        console.log(result);
        res.render(ejsPath + 'index.ejs', { test: 'success', sensorData: result});
    });

});

app.get('/pump', function(req, res){
    db.showSensorData(function (result){
        console.log("**************Showing sensor data>");
        console.log(result);
        res.render(ejsPath + 'index.ejs', { test: 'success', sensorData: result});
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