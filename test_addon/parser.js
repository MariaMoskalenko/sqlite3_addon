const Extrator = require("html-extractor");
const fs = require('fs');

var content = "";
var reduceToList0 = {
    tag: "td",
    attr: "class",
    val: "issuekey",
	list: true
};
var reduceToList1 = {
    tag: "td",
    attr: "class",
    val: "summary",
	list: true
};
var reduceToList2 = {
    tag: "td",
    attr: "class",
    val: "status",
	list: true
};

function parseFileFormQuery(fileName, outQuery) {
	content = fs.readFileSync(fileName,"utf8");
	var structure = {}; structure.id = []; structure.name = []; structure.status = [];
	var myExtrator = new Extrator();
	myExtrator.extract( content, reduceToList0, function( err, data0 ){
		if( err ){
			throw( err )
		} else {
			structure.id = data0.body;
		}
	});
	myExtrator.extract( content, reduceToList1, function( err, data1 ){
		if( err ){
			throw( err )
		} else {
			structure.name = data1.body;
		}
	});
	myExtrator.extract( content, reduceToList2, function( err, data2 ){
		if( err ){
			throw( err )
		} else {
			structure.status = data2.body;
		}
	});
	structure.id.forEach(function(item, i) {
		outQuery[i] = "INSERT INTO tickets VALUES(\"" + item + "\",";
	});
	structure.name.forEach(function(item, i) {
		outQuery[i] += "\"" + item + "\",";
	});
	structure.status.forEach(function(item, i) {
		outQuery[i] += "\"" + item + "\");";
	});
}

exports.parseFileFormQuery = parseFileFormQuery;




