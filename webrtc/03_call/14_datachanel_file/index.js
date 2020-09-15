/*
文件传输功能，新增信令如下，发送端通过socket发送此信令，接收端通过socket接收此信令
var data = {
    type : 'send-file',
    file : {
        name : xxx,
        type : xxx,
        size : xxx
    }
}

send_f()->createPeerConnection('caller')->socket.emit('message'...'send-file')->socket.on('message'...'send-file-ack')->senddata()
*/

'use strict'

let inputfile = document.querySelector('input#fileInput')
let btn_send = document.querySelector('button#sendFile')
let btn_abort = document.querySelector('button#abortSend')
let downloadAnchor = document.querySelector('a#download')

var pc;
var channel;
var socket = null;
var room = "testroom";

var pcConfig = {
    'iceServers': [{
        'urls': 'turn:lishifu.work:3478',
        'credential': "216857",
        'username': "lzxysf"
    }]
};

let offset = 0;
const chunkSize = 16 * 1024;
let file;
let recvBuffer = [];
let recvSize = 0;

//创建一个RTCPeerConnection，参数callerOrCallee的取值可为caller或callee，
//caller代表主动发起会话的一方，callee代表被动接收会话的一方
function createPeerConnection(callerOrCallee) {
    pc = new RTCPeerConnection(pcConfig);
    pc.onicecandidate = (e)=>{
        if(e.candidate){
            var data = {
                type : 'candidate',
                label : e.candidate.sdpMLineIndex,
                candidate : e.candidate.candidate
            }
            socket.emit('message', room, data);
        } else {
            console.log('this is the end candidate')
        }
    }
    channel = pc.createDataChannel('chat', {negotiated: true, id: 0});
    channel.binaryType = 'arraybuffer';
    channel.onopen = (e)=>{ //open后才能发送数据
        console.log('channel open');
        if(callerOrCallee == 'caller') {
            var data = {
                type : 'send-file',
                file : {
                    name : file.name,
                    type : file.type,
                    size : file.size
                }
            }
            socket.emit('message', room, data);
        }
    }
    channel.onclose = (e)=>{
        console.log('channel close', e);
    }
    channel.onerror = (e)=>{
        console.log('channel error');
    }
    channel.onmessage = (e)=>{
        if(callerOrCallee == 'callee') {
            recvBuffer.push(e.data);
            recvSize += e.data.byteLength;
            if(recvSize == file.size) {
                console.log('translate completed');
                socket.emit('leave', room);

                const received = new Blob(recvBuffer);
                recvBuffer = [];
                recvSize = 0;
                downloadAnchor.href = URL.createObjectURL(received);
                downloadAnchor.download = file.name;
                downloadAnchor.textContent =
                  `Click to download '${file.name}' (${file.size} bytes)`;
                downloadAnchor.style.display = 'block';
            }
        }
    }
}

window.onload = init;

function init() {
    socket = io.connect("https://lishifu.work:4430");
    socket.on('message', (room, data)=>{
        if(data.type == 'offer') {
            createPeerConnection('callee');
            var desc = new RTCSessionDescription(data.desc);
            pc.setRemoteDescription(desc);
            pc.createAnswer().then((desc)=>{
                pc.setLocalDescription(desc);
                var data = {
                    type : 'answer',
                    desc : desc
                }
                socket.emit('message', room, data);
            });
        } else if(data.type == 'answer') {
            var desc = new RTCSessionDescription(data.desc);
            pc.setRemoteDescription(desc);
        } else if(data.type == 'candidate') {
            var candidate =  new RTCIceCandidate({sdpMLineIndex: data.label, candidate: data.candidate});
            pc.addIceCandidate(candidate);
        } else if(data.type == 'send-file') {
            file = data.file;
            console.log(`File is ${[file.name, file.size, file.type].join(' ')}`);
            var data = {
                type : 'send-file-ack',
                result : 200
            }
            socket.emit('message', room, data);
        } else if(data.type == 'send-file-ack') { //在收到send-file-ack后再发送文件，此时对端已准备好（得知了要传送文件的name、size等信息）
            if(data && data.result == 200) {
                senddata();
            }
        }
    });

    socket.on('full', (room, socketid)=>{
        alert('room ' + room + ' is full');
    });

    socket.on('leaved', (room, socketid)=>{
        channel.close();
        channel = null;
        pc.close();
        pc = null;
    });

    socket.on('bye', (room, socketid)=>{
        channel.close();
        channel = null;
        pc.close();
        pc = null;
    });
    socket.emit('join', room);
}

//类似remotecall中的call()函数
function send_f() {
    file = inputfile.files[0];
    if(file == null || file == undefined || file.size == 0) {
        socket.emit('leave', room);
        return;
    }
    console.log(`File is ${[file.name, file.size, file.type, file.lastModified].join(' ')}`);

    socket.emit('join', room); //此处重复join不会出错

    createPeerConnection('caller');
    
    pc.createOffer().then((desc)=> {
        pc.setLocalDescription(desc);
        var data = {
            type : 'offer',
            desc : desc
        }
        socket.emit('message', room , data);
    });
}

function senddata() {
    let filereader = new FileReader();
    filereader.addEventListener('error', error=> console.error('error reading file:', error));
    filereader.addEventListener('abort', event=> console.log('file reading abort:', event));
    filereader.addEventListener('load', e=> {
        console.log('filereader onload' + e);
        channel.send(e.target.result);
        offset = offset + e.target.result.byteLength;
        if(offset < file.size) {
            readSlice(offset);
        }
    });

    //readSlice函数应该这样写，如果readSlice写在外面channel会在发送一定量数据后close，不知道原因，待查
    const readSlice = o => {
        console.log('readSlice ', o);
        const slice = file.slice(offset, o + chunkSize);
        filereader.readAsArrayBuffer(slice);
      };
    readSlice(0);
}

function abort_f() {
    socket.emit('leave', room);    
}

inputfile.addEventListener('input', e=>{
    let file = inputfile.files[0];
    if(file == undefined) {
        btn_send.disabled = 'disabled';
    } else {
        btn_send.disabled = '';
    }
    console.log(inputfile.files[0])
});

btn_send.addEventListener('click', send_f);

btn_abort.addEventListener('click', abort_f);

function hangup() {
    socket.emit('leave', room);
}

function send(data) {
    if(!channel) {
        alert('channel is null')
        return;
    }
    channel.send(data);
}
