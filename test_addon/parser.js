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
	try {
		fs.statSync(fileName).isFile();
		content = fs.readFileSync(fileName,"utf8");
		var structure = {}; structure.id = [""]; structure.name = [""]; structure.status = [""];
		var myExtrator = new Extrator();
		myExtrator.extract( content, reduceToList0, function( err, data0 ){
			if( err ){
				throw( err );
			} else {
				data0.body.forEach(function(item, i) {
					structure.id.push(item.replace(/("|'|&quot;)/g, ""));
				});
			}
		});
		myExtrator.extract( content, reduceToList1, function( err, data1 ){
			if( err ){
				throw( err )
			} else {
				data1.body.forEach(function(item, i) {
					structure.name.push(item.replace(/("|'|&quot;)/g, ""));
				});
			}
		});
		myExtrator.extract( content, reduceToList2, function( err, data2 ){
			if( err ){
				throw( err );
			} else {
				data2.body.forEach(function(item, i) {
					structure.status.push(item.replace(/("|'|&quot;)/g, ""));
				});
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
	catch (err) {
		console.log(err);
		return err.code;
	}

	
}

exports.parseFileFormQuery = parseFileFormQuery;




