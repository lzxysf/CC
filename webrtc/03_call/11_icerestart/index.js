/*
此版本修复了recall版本的一个问题
即通过过程中再次点击call按钮后，recall版本不会复用已存在的RTCPeerConnection，会重新创建RTCPeerConnection
此版本中当点击call按钮后，会判断RTCPeerConnection是否为空，为空则创建，不为空则不创建，之后直接调用CreateOffer进行媒体协商
同样，接收端在收到offer后，也会先判断RTCPeerConnection是否为空，为空则创建，不为空则不创建

如果在CreateOffer中携带参数iceRestart:1，ICE会重新收集候选者，也就是onicecandidate回调函数会被再次调用多次，并把candidate发送到对端
同样对端在收到offer并且setRemoteDescription后，ICE也会重新收集候选者，onicecandidate也会被再次调用多次，并把candidate发送到对端

当CreateOffer携带参数iceRestart后，生成的desp的sdp中ice-ufrag和ice-pwd会改变，当调用setLocalDescription设置desc后，ICE才会重新进行candidate收集
对端是在调用setRemoteDescription设置desc后，ICE才重新进行candidate收集，
实质是ice-ufrag和ice-pwd的改变引起的网络重协商。

在实际使用过程中应该在网络发生变化时启动iceRestart，以重新进行网络协商，
在代码编写中，是在ice连接状态变为failed时(pc.oniceconnectionstatechange被回调且pc.iceConnectionState为failed)，启动iceRestart

官方文档中建议使用新函数pc.restartIce()来进行网络重协商，但是经测试没有效果（不清楚原因，待验证）
*/

'use strict'

let local_video = document.querySelector('#preview_video')
let remote_video = document.querySelector('#playback_video')
let btn_start = document.querySelector('#start')
let btn_call = document.querySelector('#call')
let btn_hangup = document.querySelector('#hangup')
let btn_restartice = document.querySelector('#restartice')
let local_sdp = document.querySelector('textarea#local_sdp')
let remote_sdp = document.querySelector('textarea#remote_sdp')
let candidate_text = document.querySelector('#candidate')

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

                        let candidate_desc = 'foundation:' + e.candidate.foundation +
                                            ' component:' + e.candidate.component +
                                            ' address:' + e.candidate.address +
                                            ' port:' + e.candidate.port +
                                            ' protocol:' + e.candidate.protocol +
                                            ' type:' + e.candidate.type + '\r\n';
                        candidate_text.value += candidate_desc;
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
                pc.oniceconnectionstatechange = (ev)=>{
                    if(pc.iceConnectionState == 'failed') {
                        //触发ICE重新协商
                        pc.createOffer({iceRestart:1}).then((desc)=> {
                            local_sdp.value = desc.sdp;
                            pc.setLocalDescription(desc);
                            var data = {
                                type : 'offer',
                                desc : desc
                            }
                            socket.emit('message', room , data);
                        });
                    }
                }
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
    if(!pc) {
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

                let candidate_desc = 'foundation:' + e.candidate.foundation +
                                    ' component:' + e.candidate.component +
                                    ' address:' + e.candidate.address +
                                    ' port:' + e.candidate.port +
                                    ' protocol:' + e.candidate.protocol +
                                    ' type:' + e.candidate.type + '\r\n';
                candidate_text.value += candidate_desc;
            } else {
                console.log('this is the end candidate')
            }
        }
        pc.oniceconnectionstatechange = (ev)=>{
            if(pc.iceConnectionState == 'failed') {
                //触发ICE重新协商
                pc.createOffer({iceRestart:1}).then((desc)=> {
                    local_sdp.value = desc.sdp;
                    pc.setLocalDescription(desc);
                    var data = {
                        type : 'offer',
                        desc : desc
                    }
                    socket.emit('message', room , data);
                });
            }
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

function restartice() {
    pc.createOffer({iceRestart:1}).then((desc)=> {
        local_sdp.value = desc.sdp;
        pc.setLocalDescription(desc);
        var data = {
            type : 'offer',
            desc : desc
        }
        socket.emit('message', room , data);
    });
}
