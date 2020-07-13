'use strict'

let local_video = document.querySelector('#preview_video')
let remote_video = document.querySelector('#playback_video')
let btn_start = document.querySelector('#start')
let btn_call = document.querySelector('#call')
let btn_hangup = document.querySelector('#hangup')
let local_sdp = document.querySelector('textarea#local_sdp')
let remote_sdp = document.querySelector('textarea#remote_sdp')

let localstream;
var pc;

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
            localstream.getTracks().forEach((track)=>{
                pc.addTrack(track, localstream);
            });
            pc.ontrack = (ev)=>{
                remote_video.srcObject = ev.streams[0];
            }
            var desc = new RTCSessionDescription(data.desc);
            remote_sdp.value = desc.sdp;
            pc.setRemoteDescription(desc);
            pc.createAnswer().then((desc)=>{
                local_sdp.value = desc.sdp;
                pc.setLocalDescription(desc);
                var data = {
                    type : 'answer',
                    desc : desc
                }
                socket.emit('message', room, data);
            });
        } else if(data.type == 'answer') {
            var desc = new RTCSessionDescription(data.desc);
            remote_sdp.value = desc.sdp;
            pc.setRemoteDescription(desc);
        } else if(data.type == 'candidate') {
            var candidate =  new RTCIceCandidate({sdpMLineIndex: data.label, candidate: data.candidate});
            pc.addIceCandidate(candidate);
        }
    });

    socket.on('joined', (room, socketid)=>{

    });

    socket.on('otherjoined', (room, socketid)=>{

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

function start() {
    navigator.mediaDevices.getUserMedia({video:true, audio:true}).then(function(stream){
        localstream = stream;
        local_video.srcObject = localstream;
    });
    //每次start都加入(join)房间，hangup时离开(leave)房间
    socket.emit('join', room);
}

function call() {
    pc = new RTCPeerConnection(pcConfig);
    localstream.getTracks().forEach((track)=>{
        pc.addTrack(track, localstream);
    });
    pc.ontrack = (ev)=>{
        remote_video.srcObject = ev.streams[0];
    }
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
    pc.createOffer({offerToReceiveAudio:1, offerToReceiveVideo:1}).then((desc)=> {
        local_sdp.value = desc.sdp;
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
