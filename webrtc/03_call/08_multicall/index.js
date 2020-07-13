// 本demo实现多端通话
// 访问网页时onload中有三个逻辑
// 1.连接socket信令服务器，并注册信令处理回调函数
// 2.打开本地摄像头麦克风，获取localstream并显示到video标签
// 3.加入房间
//第一个用户加入房间收到joined信令，此时什么也不处理
//之后每一个用户加入房间收到otherjoined信令，此时会收到这个新用户的socketid，本地新建一个peerconnection与这个新用户进行连接
//每新加入一个用户，原先在房间内的用户会给其发送offer并接收其answer，这样就创建了一路通话

'use strict'

let local_video = document.querySelector('#preview_video')
let remote_videos = document.querySelector('#remote_videos')
let btn_hangup = document.querySelector('#hangup')
let localstream;

var socket = null;
var room = "testroom";
var map = new Map(); //将远端的socketid和本地建立的pc对应存起来
var localsocketid; //本端连接信令服务器获取到的socketid
var streamArray = [];

var pcConfig = {
    'iceServers': [{
        'urls': 'turn:lishifu.work:3478',
        'credential': "216857",
        'username': "lzxysf"
    }]
};

window.onload = init;

function init() {
    // 1.连接socket信令服务器，并注册信令处理回调函数
    socket = io.connect("https://lishifu.work:4430");
    socket.on('message', (room, data)=>{
        if(data.type == 'offer') {
            let pc = new RTCPeerConnection(pcConfig);
            map.set(data.from, pc);
            pc.onicecandidate = (e)=>{
                if(e.candidate){
                    var data0 = {
                        from : localsocketid,
                        to : data.from,
                        type : 'candidate',
                        label : e.candidate.sdpMLineIndex,
                        candidate : e.candidate.candidate
                    }
                    socket.emit('message', room, data0);
                } else {
                    console.log('this is the end candidate');
                }
            }
            localstream.getTracks().forEach((track)=>{
                pc.addTrack(track, localstream);
            });
            pc.ontrack = (ev)=>{
                for(let i = 0; i < streamArray.length; i++) {
                    if(streamArray[i].id == ev.streams[0].id) {
                        return;
                    }
                }
                let video = document.createElement('video');
                video.srcObject = ev.streams[0];
                video.autoplay = true;
                video.style.width = "300px";
                video.style.height = "220px";
                video.id = 'video' + data.from;
                remote_videos.appendChild(video);
                streamArray.push(ev.streams[0]);
            }
            var desc = new RTCSessionDescription(data.desc);
            pc.setRemoteDescription(desc);
            pc.createAnswer().then((desc)=>{
                pc.setLocalDescription(desc);
                var data1 = {
                    from : localsocketid,
                    to : data.from,
                    type : 'answer',
                    desc : desc
                }
                socket.emit('message', room, data1);
            });
        } else if(data.type == 'answer') {
            let pc = map.get(data.from);
            var desc = new RTCSessionDescription(data.desc);
            pc.setRemoteDescription(desc);
        } else if(data.type == 'candidate') {
            var candidate =  new RTCIceCandidate({sdpMLineIndex: data.label, candidate: data.candidate});
            let pc = map.get(data.from);
            pc.addIceCandidate(candidate);
        }
    });
    socket.on('joined', (room, socketid)=>{
        localsocketid = socketid;
    });
    socket.on('otherjoined', (room, socketid)=>{
        let pc = new RTCPeerConnection(pcConfig);
        map.set(socketid, pc); //将socketid和pc的对应关系保存起来
        localstream.getTracks().forEach((track)=>{
            pc.addTrack(track, localstream);
        });
        pc.ontrack = (ev)=>{
            for(let i = 0; i < streamArray.length; i++) {
                if(streamArray[i].id == ev.streams[0].id) {
                    return;
                }
            }
            let video = document.createElement('video');
            video.srcObject = ev.streams[0];
            video.autoplay = true;
            video.style.width = "300px";
            video.style.height = "220px";
            video.id = 'video' + socketid;
            remote_videos.appendChild(video);
            streamArray.push(ev.streams[0]);
        }
        pc.onicecandidate = (e)=>{
            if(e.candidate) {
                var data = {
                    from: localsocketid,
                    to: socketid,
                    type : 'candidate',
                    label : e.candidate.sdpMLineIndex,
                    candidate : e.candidate.candidate
                }
                socket.emit('message', room, data);
            } else {
                console.log('this is the end candidate')
            }
        }
        pc.createOffer().then((desc)=> {
            pc.setLocalDescription(desc);
            var data = {
                from : localsocketid,
                to : socketid,
                type : 'offer',
                desc : desc
            }
            socket.emit('message', room , data);
        });
    });
    socket.on('full', (room, socketid)=>{
        alert('room ' + room + ' is full');
    });
    socket.on('leaved', (room, socketid)=>{
        for(let [key,pc] of map) {
            pc.close();
            pc = null;
            var video_item = document.getElementById('video'+key);
            video_item.srcObject = null;
            remote_videos.removeChild(video_item);
        }
        map.clear();
    });
    socket.on('bye', (room, socketid)=>{
        let pc = map.get(socketid);
        pc.close();
        pc = null;
        var video_item = document.getElementById('video'+socketid);
        video_item.srcObject = null;
        remote_videos.removeChild(video_item);
        map.delete(socketid);
    });

    // 2.打开本地摄像头麦克风，获取localstream并显示到video标签
    navigator.mediaDevices.getUserMedia({video:true, audio:true}).then((stream)=>{
        localstream = stream;
        local_video.srcObject = localstream;
        // 3.加入房间
        socket.emit('join', room);
    });
}

function hangup() {
    socket.emit('leave', room);
}
