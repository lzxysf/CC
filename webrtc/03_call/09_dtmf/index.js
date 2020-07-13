//DTMF类似于电话通话过程中按下数字键，比如‘人工请按0’
//webrtc发送DTMF是在rtp的payloads中携带，它不是媒体数据，而是作为带外数据（out-of-band）进行传输的
//所谓的带外数据，也叫紧急数据，不像媒体数据一样经过缓冲后发送，而是直接发送给对端，它比媒体数据有更高的优先级
//另外webrtc虽然能够发送dtmf，但是确实没有提供在对端获得dtmf的方法（我他妈也很费解）

//dtmf的传输需要依赖于音频，因此本例子中在一个audiosender中进行发送
//RTCDTMFSender.insertDTMF(tones[, duration[, interToneGap]]);

'use strict'

let local_audio = document.querySelector('#local_audio')
let remote_audio = document.querySelector('#remote_audio')
let btn_start = document.querySelector('#start')
let btn_call = document.querySelector('#call')
let btn_hangup = document.querySelector('#hangup')
let btn_senddtmf = document.querySelector('#senddtmf')
let dtmftext = document.querySelector('#dtmftext')
let recvtone = document.querySelector('#recvtone')

let localstream;
var pc;

var socket = null;
var room = "testroom";

var dtmfsender;

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
                remote_audio.srcObject = ev.streams[0];
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
        remote_audio.srcObject = null;
        local_audio.srcObject = null;
        localstream = null;
    });

    socket.on('bye', (room, socketid)=>{
        pc.close();
        pc = null;
        remote_audio.srcObject = null;
        local_audio.srcObject = null;
        localstream = null;
    });
}

function start() {
    navigator.mediaDevices.getUserMedia({video:false, audio:true}).then(function(stream){
        localstream = stream;
        local_audio.srcObject = localstream;
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
        remote_audio.srcObject = ev.streams[0];
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

    // 获取RTCDtmfSender
    var audiosender = pc.getSenders().find(sender=>sender.track && sender.track.kind=='audio');
    dtmfsender = audiosender.dtmf;
    dtmfsender.ontonechange = e=>{
        if(e.tone) {
            recvtone.value = recvtone.value += e.tone;
        } else {
            console.log('tone send completed.')
        }
    }

    pc.createOffer({offerToReceiveAudio:1, offerToReceiveVideo:0}).then((desc)=> {
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

function senddtmf() {
    var text = dtmftext.value;
    dtmfsender.insertDTMF(text, 500, 150);
}