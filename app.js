'use strict'

// C library API
const ffi = require('ffi-napi');

let cLib = ffi.Library('./parser/bin/libsvgparse.so', {
  //"functionName": ["returnType", ["param1", "param2"]],
  //"writeSVGimage": ["bool", ["string", "string"]],
  "SVGtoJSON_Wrapper": ["string", ["string", "string"]],
});

// cLib.functionName()

// Express App (Routes)
const express = require("express");
const app = express();
const path = require("path");
const fileUpload = require('express-fileupload');

app.use(fileUpload());
app.use(express.static(path.join(__dirname + '/uploads')));

// Minimization
const fs = require('fs');
const JavaScriptObfuscator = require('javascript-obfuscator');

// Important, pass in port as in `npm run dev 1234`, do not change
const portNum = process.argv[2];

// Send HTML at root, do not change
app.get('/', function (req, res) {
  res.sendFile(path.join(__dirname + '/public/index.html'));
});

// Send Style, do not change
app.get('/style.css', function (req, res) {
  //Feel free to change the contents of style.css to prettify your Web app
  res.sendFile(path.join(__dirname + '/public/style.css'));
});

// Send obfuscated JS, do not change
app.get('/index.js', function (req, res) {
  fs.readFile(path.join(__dirname + '/public/index.js'), 'utf8', function (err, contents) {
    const minimizedContents = JavaScriptObfuscator.obfuscate(contents, { compact: true, controlFlowFlattening: true });
    res.contentType('application/javascript');
    res.send(minimizedContents._obfuscatedCode);
  });
});

//Respond to POST requests that upload files to uploads/ directory
app.post('/upload', function (req, res) {
  if (!req.files) {
    return res.status(400).send('No files were uploaded.');
  }

  let uploadFile = req.files.uploadFile;
  console.log("Files: " + req.files);
  fs

  // Use the mv() c to place the file somewhere on your server
  uploadFile.mv('uploads/' + uploadFile.name, function (err) {
    if (err) {
      return res.status(500).send(err);
    }

    res.redirect('/');
  });
});

//Respond to GET requests for files in the uploads/ directory
app.get('/uploads/:name', function (req, res) {
  fs.stat('uploads/' + req.params.name, function (err, stat) {
    if (err == null) {
      res.sendFile(path.join(__dirname + '/uploads/' + req.params.name));
    } else {
      console.log('Error in file downloading route: ' + err);
      res.send('');
    }
  });
});

//******************** Your code goes here ******************** 
app.get('/getFiles', function (req, res) {
  let files = fs.readdirSync('./uploads/');
  console.log('Sending the files...');
  console.log(files);
  let listOfFiles = []
  for (let x in files) {
    let fileStats = fs.statSync(__dirname + '/uploads/' + files[x]);

    let string = cLib.SVGtoJSON_Wrapper(__dirname + '/uploads/' + files[x], './parser/test/schemaFiles/svg.xsd');
    if (string.localeCompare("{}") == 0) {
      console.log('failed to validate svg image ' + files[x]);
      continue;
    }

    listOfFiles.push({
      fileName: files[x],
      size: Math.round(fileStats.size),
      SVGdata: JSON.parse(string),
    });
  }
  console.log(listOfFiles)
  res.send(listOfFiles);
});

//Sample endpoint
app.get('/someendpoint', function (req, res) {
  let retStr = req.query.name1 + " " + req.query.name2;
  res.send({
    foo: retStr
  });
});

app.listen(portNum);
console.log('Running app at localhost: ' + portNum);