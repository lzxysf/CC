//经过测试，在本地调用setLocalDescription，随后onicecandidate回调函数即被触发，即收集到了candidate
//如果不调用setLocalDescription，那么onicecandidate不会被触发

//本例中使用的stun/turn服务器为coturn搭建的，默认端口号为3478
//公开的stun服务器有:
// stun:stun1.l.google.com:19302
// stun:stun2.l.google.com:19302
// stun:stun3.l.google.com:19302
// stun:stun4.l.google.com:19302
// stun:stun01.sipphone.com
// stun:stun.ekiga.net
// stun:stun.fwdnet.net
// stun:stun.ideasip.com
// stun:stun.iptel.org
// stun:stun.rixtelecom.se
// stun:stun.schlund.de
// stun:stunserver.org
// stun:stun.softjoys.com
// stun:stun.voiparound.com
// stun:stun.voipbuster.com
// stun:stun.voipstunt.com
// stun:stun.voxgratia.org
// stun:stun.xten.com
//(大部分可用，未一一测试)
//（没有找到公开的turn服务器，turn服务器需要中继媒体流，太消耗服务器带宽资源了）

'use strict'

let btn_gather = document.querySelector('#gather')
let candidate_text = document.querySelector('#candidate')
var pc;

var pcConfig = {
    'iceServers': [{
        'urls': 'turn:lishifu.work:3478',
        'credential': "216857",
        'username': "lzxysf"
    }],
    // iceTransportPolicy : 'relay' // 只收集媒体服务器提供的中继候选地址
    // iceTransportPolicy : 'all'   // 收集所有的候选地址，默认就是手机所有的候选地址
    iceCandidatePoolSize: 0
};

function gather() {
    candidate_text.value = '';

    pc = new RTCPeerConnection(pcConfig);
    pc.onicecandidate = (e)=>{
        if(e.candidate == null){
            return;
        }
        console.log(e.candidate);
        let candidate_desc = 'foundation:' + e.candidate.foundation +
                    ' component:' + e.candidate.component +
                    ' address:' + e.candidate.address +
                    ' port:' + e.candidate.port +
                    ' protocol:' + e.candidate.protocol +
                    ' type:' + e.candidate.type + '\r\n';
        candidate_text.value += candidate_desc;
    };
    pc.createOffer({offerToReceiveAudio:1}).then((desc)=> {
        pc.setLocalDescription(desc);
    });
}
