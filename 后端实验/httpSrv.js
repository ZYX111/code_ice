var http = require('http');
var url = require('url');
var playerList;
var serv = http.createServer(function(req, res){
	res.writeHead(200, {
		'Content-Type': 'application/json;charset=utf-8',
		'Access-Control-Allow-Origin': '*',
		'Access-Control-Allow-Headers': 'X-Requested-With',
		'Access-Control-Allow-Methods': 'PUT,POST,GET,DELETE,OPTIONS',
		'X-Powered-By': '3.2.1'
	});
	var arg = url.parse(req.url, true);
	if ('/setUp'==arg.pathname) {
		req.on('data', function(data){
			var obj = JSON.parse(decodeURIComponent(data));
			playerList = obj.playerList;
		})
		req.on('end', function(){
			res.end(JSON.stringify({result: 'ok'}));
		})
	}
	if ('/getPlayerList'==arg.pathname) {
		res.end(JSON.stringify({playerList:playerList}));
	}
})

serv.listen(3000);