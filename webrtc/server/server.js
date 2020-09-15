var express = require('express')
var serveIndex = require('serve-index')
var https = require('https')
var socketio = require('socket.io')
var fs = require('fs')

//指定要显示的页面的路径
var app = express();
app.use(serveIndex('../'))
app.use(express.static('../'))

//启动一个https服务器，端口为4430（一般https的端口为443）
var options = {
	key : fs.readFileSync('./cert/2_lishifu.work.key'),
	cert: fs.readFileSync('./cert/1_lishifu.work_bundle.crt')
}
var https_server = https.createServer(options, app);
https_server.listen(4430, '0.0.0.0');

//使用https服务器启动一个socketio服务
var io = socketio.listen(https_server);

var map = new Map();

io.sockets.on('connection', (socket)=> {
    socket.on('join', room => {
        socket.join(room);
        map.set(socket.id, socket);
        var myRoom = io.sockets.adapter.rooms[room];
        var users = Object.keys(myRoom.sockets);
        console.log("房间内的用户数为" + users.length);
        if(users.length < 5) {
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
        //客户端可能多次调用了leave，在此前的leave中room中成员可能都退出了，此时这个room也会自动解散了
        if(myRoom == undefined) {
            console.log('无该room')
            return;
        }
        var users = Object.keys(myRoom.sockets);
        socket.emit('leaved', room, socket.id); //向当前客户端发送leaved消息
        socket.to(room).emit('bye', room, socket.id); //向房间内所有人（除自己外）发送bye消息
        socket.leave(room);
        map.delete(socket.id);
        console.log("房间内当前用户数为" + (users.length - 1));
    });

    socket.on('message', (room, data) => {
        console.log("收到了房间" + room + "的数据" + JSON.stringify(data));
        if(data.from && data.to) { //此分支用于多人聊天，多人聊天中携带的message信令是发送给特定对象的，from和to都是socketid
            map.get(data.to).emit('message', room, data); //如果data中有指向，就向特定的终端发送消息
            // io.sockets.connected[data.to].emit('message', room, data); //也可以用这条语句通过socketid获得socket，这样就不用自己用map维护socketid和socket的关系了
        } else {
            socket.to(room).emit('message', room, data); //房间内除自己外的所有人（一对一通信中也就是对端）发送message消息
        }
    });

    //监听正在断开事件，reason表示正在断开原因
    socket.on('disconnecting', (reason)=>{
        console.log('连接正在断开:' + 'socketid:' + socket.id + ',reason:' + reason)
    })

    //监听断开事件，当连接断开时该回调函数被执行，reason表示断开原因
    socket.on('disconnect', (reason)=>{
        console.log('连接已经断开:' + 'socketid:' + socket.id + ',reason:' + reason)
        map.delete(socket.id); //map中不再维护这个socket的关系
    })
});
