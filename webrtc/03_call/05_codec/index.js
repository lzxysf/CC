//change codecs before the call 在通话前提前设置编解码方式
//注意：上一个例子中带宽是在通话过程中设置的，但是编解码方式设置必须在通话前
//因此selector控件初始状态为disabled，start后取消disabled，call后disabled，hangup后取消disabled

'use strict'

let local_video = document.querySelector('#preview_video')
let remote_video = document.querySelector('#playback_video')
let btn_start = document.querySelector('#start')
let btn_call = document.querySelector('#call')
let btn_hangup = document.querySelector('#hangup')
let local_sdp = document.querySelector('textarea#local_sdp')
let remote_sdp = document.querySelector('textarea#remote_sdp')
let codecsselector = document.querySelector('#codecsselector')

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
        codecsselector.disabled = false;
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

    codecsselector.disabled = true;
}

function start() {
    let stream = navigator.mediaDevices.getUserMedia({video:true, audio:false}).then(function(stream){
        localstream = stream;
        local_video.srcObject = stream;
    });

    addSupportCodecs();
    codecsselector.disabled = false;
}

//获取视频编解码的能力列表并添加到界面
//RTCRtpSender的静态方法getCapabilitie可以获得媒体能力，它的返回值为RTCRtpCapabilities 
//返回值RTCRtpCapabilities格式如 codecs: Array(10), headerExtensions: Array(9)
//因此要获得 codecs 应当使用 const {codecs} = RTCRtpSender.getCapabilities('video')
function addSupportCodecs() {
    const {codecs} = RTCRtpSender.getCapabilities('video');
    codecs.forEach((codec)=>{
        //red fec都是种冗余的机制，rtx是重传的机制，这些不加入界面列表中
        if(codec.mimeType == 'video/red' || codec.mimeType == 'video/ulpfec' || codec.mimeType == 'video/rtx') {
            return;
        }
        let value = codec.mimeType + ' ' + (codec.sdpFmtpLine || '').trim(); //codec可能无参数sdpFmtpLine
        let option = document.createElement('option');
        option.value = value;
        option.innerHTML = value;
        codecsselector.appendChild(option);
    });
}

//注意，由于改变codecs要在通话之前，因此此函数必须在CreateOffer()之前调用
function changeCodecsBeforeCall() {
    const [mimeType, sdpFmtpLine] = codecsselector.value.split(' ');
    var {codecs} = RTCRtpSender.getCapabilities('video');
    var selectedIndex = codecs.findIndex(codec=>codec.mimeType == mimeType && codec.sdpFmtpLine == sdpFmtpLine);
    var selectedCodec = codecs[selectedIndex];
    //将选中的codec移动到整个codecs数组的最前面
    codecs.splice(selectedIndex, 1);
    codecs.unshift(selectedCodec);

    const transceiver = pc.getTransceivers().find(t => t.sender && t.sender.track === localstream.getVideoTracks()[0]);
    transceiver.setCodecPreferences(codecs);
}

function call() {
    pc = new RTCPeerConnection();
    localstream.getTracks().forEach((track)=>{
        pc.addTrack(track, localstream);
    });

    changeCodecsBeforeCall();

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

    addSupportCodecs.disabled = true;
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
