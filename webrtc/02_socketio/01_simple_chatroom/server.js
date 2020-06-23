var express = require('express')
var serveIndex = require('serve-index')
var http = require('http')
var socketio = require('socket.io')

//指定要显示的页面的路径
var app = express();
app.use(serveIndex('./'))
app.use(express.static('./'))

//启动一个http服务器，端口为3000
var http_server = http.createServer(app)
http_server.listen(3000, '0.0.0.0')

//使用http服务器启动一个socketio服务
var io = socketio.listen(http_server)

io.sockets.on('connection', (socket)=> {
    
    socket.on('join', room => {
        socket.join(room);
        var myRoom = io.sockets.adapter.rooms[room];
        var users = Object.keys(myRoom.sockets);
        console.log("房间内的用户数为" + users.length);
        socket.emit('joined', room, socket.id); //向当前客户端发送joined消息
        // socket.to(room).emit('joined', room, socket.id); //向房间内所有人（除自己外）发送消息
        // io.in(room).emit('joined', room, socket.id); //向房间内所有人（包括自己）发送消息
        // socket.broadcast.emit('joined', room, socket.id); //除自己全部站点
    });

    socket.on('leave', room => {
        var myRoom = io.sockets.adapter.rooms[room];
        var users = Object.keys(myRoom.sockets);
        socket.leave(room);
        socket.to(room).emit('bye', room, socket.id); //向房间内所有人（除自己外）发送bye消息
        socket.emit('leaved', room, socket.id); //向自己发送leaved消息
        console.log("房间内当前用户数为" + users.length);
    });

    socket.on('message', (room, data) => {
        console.log("收到了房间" + room + "的数据" + data);
        io.in(room).emit('message', room, data); //房间内所有人(包括自己)发送message消息
    });
});
