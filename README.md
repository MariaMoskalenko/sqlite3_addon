Experimantal sqlite3_addon.

Include is via require:
const addon = require('sqlite3_addon');

Provides for js such calls:
- create new object:
	var obj = new addon.CommandQueue();
- open database:
	var res = obj.openDB("somedb");
- execute some statement: 
	var res = obj.executeQuery("some statement");
- close database:
	var res = obj.closeDB();

Structure:
sqlite3_addon/ - addon source folder;
test_addon/ - examples of usage from js.

How to build:
- precondiditions: should be installed and configured NodeJS( https://nodejs.org/en/ , includes npm ) and node-gyp( https://github.com/nodejs/node-gyp );
- install: from test_addon folder in console "npm install".

How to test:
- in test_addon index.js http server is started on port 3000, so just open in browser http://127.0.0.1:3000/ .

Used example from: https://www.tutorialspoint.com/sqlite/sqlite_c_cpp.htm .
