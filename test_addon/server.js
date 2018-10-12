const http = require('http');
const port = 3000;
const url = require('url');
const addon = require('sqlite3_addon');
const parser = require("./parser");

var queryCreate = "CREATE TABLE IF NOT EXISTS tickets(id UNIQUE, name, status);";
var querysToInsert = [];
var querySelect = "SELECT * from tickets";

function formQueryToInsertAndExecute(instance, fileName, outQuery) {
	parser.parseFileFormQuery(fileName, outQuery);
	outQuery.forEach(function(item, i) {
		var result = instance.executeQuery(item);
		console.log( "==>" + result );
	});
}

const requestHandler = (request, response) => {
	if (request.url != '/favicon.ico') {
		console.log("call new " + request.url);
		var obj = new addon.CommandQueue();
		var res = obj.openDB("myDatabase.db");
		console.log("==> 1 " + res);
		res = obj.executeQuery(queryCreate);
		console.log("==> 2 " + res);
		formQueryToInsertAndExecute(obj, "output.html", querysToInsert);
		formQueryToInsertAndExecute(obj, "output2.html", querysToInsert);
		formQueryToInsertAndExecute(obj, "output3.html", querysToInsert);
		res = obj.executeQuery(querySelect);
		//console.log("==> 3 " + res);
		obj.closeDB();
		response.writeHead(200, {'Content-Type': 'text/javascript'});
		response.write(res.toString() + " ");
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




