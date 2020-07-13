var https = require('https')
var socketio = require('socket.io')
var fs = require('fs')

//启动一个https服务器
var options = {
	key : fs.readFileSync('./cert/2_lishifu.work.key'),
	cert: fs.readFileSync('./cert/1_lishifu.work_bundle.crt')
}
var https_server = https.createServer(options);
https_server.listen(4430, '0.0.0.0');

//使用https服务器启动一个socketio服务
var io = socketio.listen(https_server);

io.sockets.on('connection', (socket)=> {
    
    socket.on('join', room => {
        socket.join(room);
        var myRoom = io.sockets.adapter.rooms[room];
        var users = Object.keys(myRoom.sockets);
        console.log("房间内的用户数为" + users.length);
        if(users.length < 3) {
            socket.emit('joined', room, socket.id); //向当前客户端发送joined消息
            if(users.length > 1) { //如果房间内有其它人
                socket.to(room).emit('otherjoined', room ,socket.id); //向当前房间的所有人（除自己外）发送otherjoined消息
            }
        } else {
            socket.leave(room);
            socket.emit('full', room, socket.id); //向当前客户端发送full消息
        }
    });

    socket.on('leave', room => {
        var myRoom = io.sockets.adapter.rooms[room];
        var users = Object.keys(myRoom.sockets);
        socket.emit('leaved', room, socket.id); //向当前客户端发送leaved消息
        socket.to(room).emit('bye', room, socket.id); //向房间内所有人（除自己外）发送bye消息
        socket.leave(room);
        console.log("房间内当前用户数为" + users.length);
    });

    socket.on('message', (room, data) => {
        console.log("收到了房间" + room + "的数据" + JSON.stringify(data));
        socket.to(room).emit('message', room, data); //房间内除自己外的所有人（一对一通信中也就是对端）发送message消息
        var type = data.type;
    });
});
