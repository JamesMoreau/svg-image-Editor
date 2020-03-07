'use strict'

// C library API
const ffi = require('ffi-napi');

var cLib = ffi.Library('./parser/bin/libsvgparse.so', {
  //"functionName": ["returnType", ["param1", "param2"]],
  //"writeSVGimage": ["bool", ["string", "string"]],
  "SVGtoJSON_Wrapper": ["string", ["string", "string"]],
  "SVG_get_title_Wrapper": ["string", ["string", "string"]],
  "SVG_get_title_Wrapper": ["string", ["string", "string"]],
  "SVG_get_description_Wrapper": ["string", ["string", "string"]],
  "rectListToJSON_Wrapper": ["string", ["string", "string"]],
  "circListToJSON_Wrapper": ["string", ["string", "string"]],
  "pathListToJSON_Wrapper": ["string", ["string", "string"]],
  "groupListToJSON_Wrapper": ["string", ["string", "string"]],
  "attrListToJSON_Wrapper": ["string", ["string", "string"]],
  "validateSVGimage_Wrapper": ["bool", ["string", "string"]],
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
  
  //checks
  
  if (uploadFile == undefined) {
    console.log("upload file is undefined!");
    return res.status(400).send('upload file is undefined!');
  }
  // console.log(uploadFile.name.split('.').pop());

  if (!cLib.validateSVGimage_Wrapper(__dirname + '/uploads/' + uploadFile.name, './parser/test/schemaFiles/svg.xsd')) {
    console.log('svg file was not valid');
    return res.status(400).send('svg file was not valid.');
  }

  let files = fs.readdirSync('./uploads/');
  for (let x in files) {
    if (uploadFile.name.localeCompare(files[x]) == 0) {
      console.log('upload file: ' + uploadFile.name + ' files[x]' + files[x]);
      console.log('svg was already on the server!');
      return res.status(400).send('file already uploaded!.');
    }
  }  

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
  let listOfFiles = [];
  for (let x in files) {
    let fileStats = fs.statSync(__dirname + '/uploads/' + files[x]);

    let string = cLib.SVGtoJSON_Wrapper(__dirname + '/uploads/' + files[x], './parser/test/schemaFiles/svg.xsd');
    if (string.localeCompare("{}") == 0) {
      console.log('failed to validate svg image ' + files[x]);
      continue;
    }

    listOfFiles.push({
      fileName: files[x],
      size: Math.round(fileStats.size / 1000),
      SVGdata: JSON.parse(string),
    });
  }
  console.log(listOfFiles)
  res.send(listOfFiles);
});

app.get('/getFileData', function (req, res) {
  let fileName = req.query.fileName;
  console.log(fileName)

  let title_string = cLib.SVG_get_title_Wrapper(__dirname + '/uploads/' + fileName, './parser/test/schemaFiles/svg.xsd');
  if (title_string.localeCompare("") == 0) {
    console.log('No title for file.');
  } else {
    console.log(title_string);
  }

  let desc_string = cLib.SVG_get_description_Wrapper(__dirname + '/uploads/' + fileName, './parser/test/schemaFiles/svg.xsd');
  if (desc_string.localeCompare("") == 0) {
    console.log("No description for file.");
  } else {
    console.log(desc_string);
  }

  let rect_list_string = cLib.rectListToJSON_Wrapper(__dirname + '/uploads/' + fileName, './parser/test/schemaFiles/svg.xsd');
  if (rect_list_string.localeCompare("[]") == 0) {
    console.log("SVG has no rectangles.");
  } else {
    console.log(rect_list_string);
  }
  
  let circ_list_string = cLib.circListToJSON_Wrapper(__dirname + '/uploads/' + fileName, './parser/test/schemaFiles/svg.xsd');
  if (circ_list_string.localeCompare("[]") == 0) {
    console.log("SVG has no circles.");
  } else {
    console.log(circ_list_string);
  }

  let path_list_string = cLib.pathListToJSON_Wrapper(__dirname + '/uploads/' + fileName, './parser/test/schemaFiles/svg.xsd');
  if (path_list_string.localeCompare("[]") == 0) {
    console.log("SVG has no paths.");
  } else {
    console.log(path_list_string);
  }

  let attr_list_string = cLib.attrListToJSON_Wrapper(__dirname + '/uploads/' + fileName, './parser/test/schemaFiles/svg.xsd');
  if (attr_list_string.localeCompare("[]") == 0) {
    console.log("SVG has no attribute.");
  } else {
    console.log(attr_list_string);
  }

  let group_list_string = cLib.groupListToJSON_Wrapper(__dirname + '/uploads/' + fileName, './parser/test/schemaFiles/svg.xsd');
  if (group_list_string.localeCompare("[]") == 0) {
    console.log("SVG has no attribute.");
  } else {
    console.log(group_list_string);
  }

  res.send({
    fileName: fileName,
    title: title_string,
    description: desc_string,
    rectList: JSON.parse(rect_list_string),
    circList: JSON.parse(circ_list_string),
    pathList: JSON.parse(path_list_string),
    groupList:JSON.parse(group_list_string),
    attrList: JSON.parse(attr_list_string),
  });
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