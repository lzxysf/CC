'use strict'

let local_video = document.querySelector('#preview_video')
let remote_video = document.querySelector('#playback_video')
let btn_start = document.querySelector('#start')
let btn_call = document.querySelector('#call')
let btn_turnonvideo = document.querySelector('#turnonvideo');
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
        } else if(data.type == 'turn_on_video_offer') {
            navigator.mediaDevices.getUserMedia({video:true}).then(stream=>{
                var videoTracks = stream.getVideoTracks();
                if(videoTracks.length > 0) {
                    console.log('using video device:' + videoTracks[0].label);
                }
                localstream.addTrack(videoTracks[0]);
                pc.addTrack(videoTracks[0], localstream);
                local_video.srcObject = null;
                local_video.srcObject = localstream;

                var desc = new RTCSessionDescription(data.desc);
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
    let stream = navigator.mediaDevices.getUserMedia({video:false, audio:true}).then(function(stream){
        localstream = stream;
        local_video.srcObject = stream;
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
    pc.createOffer({offerToReceiveAudio:1, offerToReceiveVideo:0}).then((desc)=> {
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

function turnonvideo() {
    navigator.mediaDevices.getUserMedia({video:true}).then(function(stream){
        var videoTracks = stream.getVideoTracks();
        if(videoTracks.length > 0) {
            console.log('using video device:' + videoTracks[0].label);
        }
        localstream.addTrack(videoTracks[0]);
        pc.addTrack(videoTracks[0], localstream);

        local_video.srcObject = null;
        local_video.srcObject = localstream;
        
        return pc.createOffer();
    }).then((desc)=> {
        local_sdp.value = desc.sdp;
        pc.setLocalDescription(desc);
        var data = {
            type : 'turn_on_video_offer',
            desc : desc
        }
        socket.emit('message', room , data);
    });
}
