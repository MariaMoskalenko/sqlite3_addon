const http = require('http');
const port = 3000;
const url = require('url');
const addon = require('sqlite3_addon');

var queryCreate = 	"CREATE TABLE IF NOT EXISTS COMPANY(ID INT PRIMARY KEY NOT NULL, NAME TEXT NOT NULL,\
					 AGE INT NOT NULL, ADDRESS CHAR(50), SALARY REAL );";
var querysToInsert = "INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY)\
					 VALUES (1, 'Paul', 32, 'California', 20000.00 );\
					 INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY)\
					 VALUES (2, 'Allen', 25, 'Texas', 15000.00 );\
					 INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY)\
					 VALUES (3, 'Teddy', 23, 'Norway', 20000.00 );\
					 INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY)\
					 VALUES (4, 'Mark', 25, 'Rich-Mond ', 65000.00 );";
var querySelect = 	"SELECT * from COMPANY";
var queryUpdate = 	"UPDATE COMPANY set SALARY = 25000.00 where ID=1;\
					SELECT * from COMPANY";
var queryDelete =   "DELETE from COMPANY where ID=2;\
				    SELECT * from COMPANY";

const requestHandler = (request, response) => {
	if (request.url != '/favicon.ico') {
		console.log("call new " + request.url);
		var resQuery = {}; var str = "";
		var obj = new addon.CommandQueue();
		var res = obj.openDB("test.db");
		console.log("==> 1 " + res);
		res = obj.executeQuery(queryCreate);
		console.log("==> 2 " + res);
		res = obj.executeQuery(querysToInsert);
		console.log("==> 3 " + res);
		resQuery = obj.executeQuery(querySelect);
		str += "Select: === 4 ===\n";
		for(var i in resQuery) {
			str += resQuery[i] + '\n';
		}
		console.log("==> 4 " + str);
		str += "=== 4 ===\n";
	    resp = obj.executeQuery(queryUpdate);
		str += "Update: === 5 ===\n";
		for(var i in resQuery) {
			str += resQuery[i] + '\n';
		}
		console.log("==> 5 " + str);
		str += "=== 5 ===\n";
		resp = obj.executeQuery(queryDelete);
		str += "Delete: === 6 ===\n";
		for(var i in resQuery) {
			str += resQuery[i] + '\n';
		}
		console.log("==> 6 " + str);
		str += "=== 6 ===\n";
		obj.closeDB();
		response.writeHead(200, {'Content-Type': 'text/javascript'});
		response.write(str.toString() + " ");
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




