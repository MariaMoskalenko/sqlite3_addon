const http = require('http');
const port = 3000;
const url = require('url');
const addon = require('sqlite3_addon');
const parser = require("./parser");

var queryCreate = "CREATE TABLE IF NOT EXISTS tickets(id UNIQUE, name, status);";
var queryCheck = "SELECT * FROM tickets WHERE id IS NOT NULL;";
var querysToInsert = [];
var querySelect = "SELECT * from tickets";

function formQueryToInsertAndExecute(instance, fileName, outQuery) {
	var reslt = parser.parseFileFormQuery(fileName, outQuery);
	if ('ENOENT' != reslt) {
		outQuery.forEach(function(item, i) {
			var result = instance.executeQuery(item);
			console.log( "==>" + result );
		});
	}
	else {
		return reslt;
	}
}

const requestHandler = (request, response) => {
	if (request.url != '/favicon.ico') {
		console.log("call new " + request.url);
		var obj = new addon.CommandQueue();
		var res = obj.openDB("myDatabase.db");
		var resQuery = {}; var str = "";
		console.log("==> 1 " + res);
		res = obj.executeQuery(queryCreate);
		console.log("==> 2 " + res);
		resQuery = obj.executeQuery(queryCheck);
		if (Object.keys(resQuery).length === 0) {
			console.log("==> 3 empty obj, inserting data");
			res = formQueryToInsertAndExecute(obj, "output.html", querysToInsert);
			res = formQueryToInsertAndExecute(obj, "output2.html", querysToInsert);
			res = formQueryToInsertAndExecute(obj, "output3.html", querysToInsert);
			console.log("==> 3 " + res);
		}
		else {
			console.log("==> 3 obj is not empty");
			resQuery = {};
		}
		if ('ENOENT' != res) {
			resQuery = obj.executeQuery(querySelect);
			str += "Select: === 4 ===\n";
			for(var i in resQuery) {
				str += resQuery[i] + '\n';
			}
			str += "=== 4 ===\n";
			//str = JSON.stringify(resQuery).toString();
		}
		else {
			res += " : cannot open file";
			console.log("==> 4 error: " + res);
			
		}
		obj.closeDB();
		response.writeHead(200, {'Content-Type': 'text/javascript'});
		response.write(str + " ");
		response.end();
	}
	else {
		console.log("we don't need this call from " + request.url);
	}
}

const server = http.createServer(requestHandler);

server.listen(port, (err) => {
    if (err) {
        return console.log('something bad happened', err)
    }
    console.log(`server is listening on ${port}`)
})




