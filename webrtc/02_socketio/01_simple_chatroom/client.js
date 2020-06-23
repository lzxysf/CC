'use strict'

var userName = document.querySelector('input#username');
var inputRoom = document.querySelector('input#room');
var btnConnect = document.querySelector('button#connect');
var btnLeave = document.querySelector('button#leave');
var outputArea = document.querySelector('textarea#output');
var inputArea = document.querySelector('textarea#input');
var btnSend = document.querySelector('button#send');

var socket;
var room;

btnConnect.onclick = ()=>{

    if(userName.value == "" || inputRoom.value == "") {
        alert("请输入用户名和房间号");
        return;
    }

	//connect
	socket = io.connect("ws://140.143.188.52:3000"); 
	
	//recieve message
	socket.on('joined', (room, id) => {
        console.log("加入房间" + room);
        console.log("房间号为" + id);
		btnConnect.disabled = true;
		btnLeave.disabled = false;
		inputArea.disabled = false;
		btnSend.disabled = false;
	});	
	
	socket.on('leaved', (room, id) => {
        console.log("离开房间" + room);
        console.log("房间号为" + id);
		btnConnect.disabled = false;
		btnLeave.disabled = true;
		inputArea.disabled = true;
		btnSend.disabled = true;

        //离开房间之后断开连接
		socket.disconnect();
	});

	socket.on('message', (room, data) => {
		outputArea.scrollTop = outputArea.scrollHeight;//窗口总是显示最后的内容
		outputArea.value = outputArea.value + data + '\r';
	});	

	socket.on('disconnect', (socket)=>{
		btnConnect.disabled = false;
		btnLeave.disabled = true;
		inputArea.disabled = true;
		btnSend.disabled = true;
	});

    //send message
    //连接上服务器后加入房间
    //应该先注册事件再加入房间，否则可能joined等事件立即到达无法捕获
	room = inputRoom.value;
	socket.emit('join', room);
}

btnSend.onclick = ()=>{
	var data = inputArea.value;
	data = userName.value + ':' + data;
	socket.emit('message', room, data);
	inputArea.value = '';
}

btnLeave.onclick = ()=>{
	room = inputRoom.value;
	socket.emit('leave', room);
}

inputArea.onkeypress = (event)=> {
        //event = event || window.event;
        if (event.keyCode == 13) { //回车发送消息
        var data = inputArea.value;
        data = userName.value + ':' + data;
        socket.emit('message', room, data);
        inputArea.value = '';
        event.preventDefault();//阻止默认行为
    }
}
