'use strict'

let local_video = document.querySelector('#preview_video')
let remote_video = document.querySelector('#playback_video')
let btn_start = document.querySelector('#start')
let btn_call = document.querySelector('#call')
let btn_hangup = document.querySelector('#hangup')
let local_sdp = document.querySelector('textarea#local_sdp')
let remote_sdp = document.querySelector('textarea#remote_sdp')

let localstream;
var pc1;
var pc2;

function start() {
    let stream = navigator.mediaDevices.getUserMedia({video:true, audio:false}).then(function(stream){
        localstream = stream;
        local_video.srcObject = stream;
    });
}

function call() {
    pc1 = new RTCPeerConnection();
    pc2 = new RTCPeerConnection();
    pc1.onicecandidate = (e)=>{
        pc2.addIceCandidate(e.candidate);
    }
    pc2.onicecandidate = (e)=>{
        pc1.addIceCandidate(e.candidate);
    }
    pc1.iceconnectionstatechange = (e)=>{
        console.log('pc1 ICE state');
    }
    pc2.iceconnectionstatechange = (e)=>{
        console.log('pc2 ICE state');
    }

    localstream.getTracks().forEach((track)=>{
        // pc1.addTrack(track, localstream);
        pc1.addTrack(track);
    });
    pc2.ontrack = (ev)=>{
        // remote_video.srcObject = e.streams[0];
        if (ev.streams && ev.streams[0]) {
            remote_video.srcObject = ev.streams[0];
          } else {
            let inboundStream = new MediaStream();
            inboundStream.addTrack(ev.track);
            remote_video.srcObject = inboundStream;
          }
    }

    pc1.createOffer({offerToReceiveAudio:0, offerToReceiveVideo:1}).then((desc)=> {
        pc1.setLocalDescription(desc);
        local_sdp.value = desc.sdp;

        pc2.setRemoteDescription(desc);
        pc2.createAnswer().then((desc)=> {
            pc2.setLocalDescription(desc);
            
            pc1.setRemoteDescription(desc);
            remote_sdp.value = desc.sdp;
        });
    });
}

function hangup() {
    pc1.close();
    pc2.close();
    pc1 = null;
    pc2 = null;
}
