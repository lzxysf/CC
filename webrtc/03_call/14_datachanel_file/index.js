'use strict'

let inputfile = document.querySelector('input#fileInput')
let btn_send = document.querySelector('button#sendFile')
let btn_abort = document.querySelector('button#abortSend')

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

window.onload = init;

function init() {
    socket = io.connect("https://lishifu.work:4430");
    socket.on('message', (room, data)=>{
        if(data.type == 'offer') {
            if(!pc) {
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
                pc.ondatachannel = (e)=>{
                    channel = e.channel;
                    channel.onopen = (e)=>{
                        console.log('channel open');
                    }
                    channel.onclose = (e)=>{
                        console.log('channel close')
                    }
                    channel.onerror = (e)=>{
                        console.log('channel error')
                    }
                    channel.onmessage = (e)=>{
                        console.log(e.data)
                    }
                }
            }
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
        }
    });

    socket.on('full', (room, socketid)=>{
        alert('room ' + room + ' is full');
    });

    socket.on('leaved', (room, socketid)=>{
        pc.close();
        pc = null;
        remote_video.srcObject = null;
        local_video.srcObject = null;
        localstream = null;
    });

    socket.on('bye', (room, socketid)=>{
        pc.close();
        pc = null;
        remote_video.srcObject = null;
        local_video.srcObject = null;
        localstream = null;
    });
}

function call() {
    socket.emit('join', room);
    if(!pc){
        pc = new RTCPeerConnection(pcConfig);
        pc.onicecandidate = (e)=>{
            if(e.candidate) {
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
        channel = pc.createDataChannel('chat');
        channel.onopen = (e)=>{
            console.log('channel open');
        }
        channel.onclose = (e)=>{
            console.log('channel close')
        }
        channel.onerror = (e)=>{
            console.log('channel error')
        }
        channel.onmessage = (e)=>{
            chattext.value +=  e.data + '\n';
        }
    }
    pc.createOffer().then((desc)=> {
        pc.setLocalDescription(desc);
        var data = {
            type : 'offer',
            desc : desc
        }
        socket.emit('message', room , data);
    });
}

function hangup() {
    socket.emit('leave', room);
}

function send(text) {
    if(!channel) {
        alert('channel is null')
        return;
    }
    channel.send(text);
}
