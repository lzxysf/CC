// 获取发送的统计报告，并显示发送数据的比特率和每秒发送的包数量
// RTCPeerConnection.getSenders()[0].getStats() 是获取发送的统计报告，得到的是RTCStatsReport是一个字典

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

var bitrateSeries = null;
var bitrateGraph = null;
var packetSeries = null;
var packetGraph = null;

var lastReports = null;

window.onload = init;
function init() {
    socket = io.connect("https://lishifu.work:4430");
    socket.on('message', (room, data)=>{
        if(data.type == 'offer') {
            pc = new RTCPeerConnection();
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
    });

    socket.on('bye', (room, socketid)=>{
        pc.close();
        pc = null;
    });

    socket.emit('join', room);

    bitrateSeries = new TimelineDataSeries();
    bitrateGraph = new TimelineGraphView('bitrateGraph', 'bitrateCanvas');
    bitrateGraph.updateEndDate();

    packetSeries = new TimelineDataSeries();
    packetGraph = new TimelineGraphView('packetGraph', 'packetCanvas');
    packetGraph.updateEndDate();
}

function start() {
    let stream = navigator.mediaDevices.getUserMedia({video:true, audio:false}).then(function(stream){
        localstream = stream;
        local_video.srcObject = stream;
    });
}

function call() {
    pc = new RTCPeerConnection();
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
    pc.createOffer({offerToReceiveAudio:0, offerToReceiveVideo:1}).then((desc)=> {
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

//间隔1s绘制比特率和包/s曲线
window.setInterval(()=>{
    if(!pc){
        return;
    }
    const sender = pc.getSenders()[0];
    if(!sender) {
        return;
    }
    sender.getStats().then(reports=>{
        reports.forEach(report=>{
            let bytes;
            let packets;
            if(report.type == 'outbound-rtp') {
                console.log(report);
                if(report.isRemote) {
                    return;
                }
                if(lastReports && lastReports.has(report.id))
                {
                    const bitrate = 8 * (report.bytesSent - lastReports.get(report.id).bytesSent) / (report.timestamp - lastReports.get(report.id).timestamp);
                    const packets = report.packetsSent - lastReports.get(report.id).packetsSent;

                    bitrateSeries.addPoint(report.timestamp, bitrate);
                    bitrateGraph.setDataSeries([bitrateSeries]);
                    bitrateGraph.updateEndDate();

                    packetSeries.addPoint(report.timestamp, packets);
                    packetGraph.setDataSeries([packetSeries]);
                    packetGraph.updateEndDate();
                }
            }
        });
        lastReports = reports;
    });
}, 1000);
