//通话过程中改变带宽 change bandwidth on the fly
//修改bandwidth值会影响实际发送的比特率和每秒发送的媒体包数量

//方式1：使用 RTCRtpSender.setParameters 在不重新进行本地协商的情况下改变带宽，
//但是要注意，通过 RTCRtpSender.setParameters 设置的最大带宽要在通过SDP协商的初始最大带宽的范围内。

//方式2：通过在本地进行重协商来限制带宽，也就是改变sdp并重新设置sdp

'use strict'

let local_video = document.querySelector('#preview_video')
let remote_video = document.querySelector('#playback_video')
let btn_start = document.querySelector('#start')
let btn_call = document.querySelector('#call')
let btn_hangup = document.querySelector('#hangup')
let local_sdp = document.querySelector('textarea#local_sdp')
let remote_sdp = document.querySelector('textarea#remote_sdp')
let bandwidthselector = document.querySelector('#bandwidthselector')

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

//修改bandwidth值会影响实际发送的比特率和每秒发送的媒体包数量

//方式1：使用 RTCRtpSender.setParameters 在不重新进行本地协商的情况下改变带宽，
//但是要注意，通过 RTCRtpSender.setParameters 设置的最大带宽要在通过SDP协商的初始最大带宽的范围内。
// bandwidthselector.onchange = ()=>{
//     var bandwidth = bandwidthselector.value;

//     const sender = pc.getSenders()[0];
//     const parameters = sender.getParameters();
//     if(!parameters.encodings) {
//         parameters.encodings = [{}];
//     }
//     if(bandwidth == 'unlimited') {
//         delete parameters.encodings[0].maxBitrate;
//     } else {
//         parameters.encodings[0].maxBitrate = bandwidth * 1000;
//     }
//     sender.setParameters(parameters);
// }

//方式2：通过在本地进行重协商来限制带宽
bandwidthselector.onchange = ()=>{
    var bandwidth = bandwidthselector.value;
    
    pc.createOffer({offerToReceiveAudio:0, offerToReceiveVideo:1}).then((desc)=> {
        pc.setLocalDescription(desc);
        updateBandwidthRestriction(pc.remoteDescription.sdp, bandwidth);
        pc.setRemoteDescription({
            type: pc.remoteDescription.type,
            sdp: bandwidth == 'unlimited' ? removeBandwidthRestriction(pc.remoteDescription.sdp) : updateBandwidthRestriction(pc.remoteDescription.sdp, bandwidth)
        });
    });
}

function updateBandwidthRestriction(sdp, bandwidth) {
    let modifier = 'AS';
    if (adapter.browserDetails.browser === 'firefox') {
        bandwidth = (bandwidth >>> 0) * 1000;
        modifier = 'TIAS';
    }
    if (sdp.indexOf('b=' + modifier + ':') === -1) {
        // insert b= after c= line.
        sdp = sdp.replace(/c=IN (.*)\r\n/, 'c=IN $1\r\nb=' + modifier + ':' + bandwidth + '\r\n');
    } else {
        sdp = sdp.replace(new RegExp('b=' + modifier + ':.*\r\n'), 'b=' + modifier + ':' + bandwidth + '\r\n');
    }
    remote_sdp.value = sdp;
    return sdp;
}

function removeBandwidthRestriction(sdp) {
    return sdp.replace(/b=AS:.*\r\n/, '').replace(/b=TIAS:.*\r\n/, '');
}
