'use strict'

let preview_video = document.querySelector('preview_video')
let playback_video = document.querySelector('playback_video')
let start = document.querySelector('start')
let call = document.querySelector('call')
let hangup = document.querySelector('hangup')

let audiostream = null;

async function start() {
    let stream = await navigator.mediaDevices.getUserMedia({video:true, audio:false});
    audiostream = stream;
    preview_video.srcObject = stream;
}

function call() {

}

function hangup() {
    
}