var https = require('https')
var socketio = require('socket.io')
var fs = require('fs')

//启动一个https服务器，端口为4430（一般https的端口为443）
var options = {
	key : fs.readFileSync('./cert/2_lishifu.work.key'),
	cert: fs.readFileSync('./cert/1_lishifu.work_bundle.crt')
}
var https_server = https.createServer(options);
https_server.listen(4433, '0.0.0.0');

//使用https服务器启动一个socketio服务
var io = socketio.listen(https_server);

io.sockets.on('connection', (socket)=> {
    socket.emit('message', 'welcome');
    console.log('a new connectioin arrvie:' + socket.id);

    socket.on('join', room => {
        socket.join(room);
    });

    socket.on('leave', room => {
        socket.leave(room);
    });

    socket.on('request', (data) => {
        socket.emit('response', data);
    });

    //socket.io内置事件，正在断开连接时触发，reason表示正在断开原因
    socket.on('disconnecting', (reason)=>{
        console.log('disconnecting,' + 'socketid:' + socket.id + ',reason:' + reason);
    });

    //socket.io内置事件，断开连接时触发，reason表示断开原因
    socket.on('disconnect', (reason)=>{
        console.log('disconnect,' + 'socketid:' + socket.id + ',reason:' + reason);
    });
});
