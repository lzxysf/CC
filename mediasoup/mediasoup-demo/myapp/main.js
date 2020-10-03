const mediasoupClient = require('mediasoup-client');
const io = require('socket.io-client')

let device;
let sendTransport;
let recvTransport;
let udn;
let password;

let consumers = new Map();
let producers = new Map();
let dataproducers = new Map();
let dataconsumers = new Map();

console.log('mediasoup version:%s', mediasoupClient.version);

try {
    device = new mediasoupClient.Device();
} catch (error) {
    if(error.name == 'UnsupportedError') {
        console.warn('browser not supported');
    }
}

var taskid = 1;
function getTaskID() {
    return taskid++;
}
function requestToServer(requestType, data) {
    return new Promise((resolve, reject)=>{
        let taskid = getTaskID();
        let req_data = {
            type: requestType,
            taskid: taskid,
            data: data
        }
        socket.on('response', response=>{
            if(t1 && response.type == requestType && response.taskid == taskid) {
                clearTimeout(t1);
                if(response.type != 'heartbeat') {
                    console.log('-----response', response);
                }
                resolve(response);
            }
        });
        let t1 = setTimeout(() => {
            t1 = null;
            resolve('timeout');
        }, 1000);
        if(requestType != 'heartbeat') {
            console.log('-----request', req_data);
        }
        socket.emit('request', req_data);
    });
}

var socket = io('wss://www.lishifu.work:4431');
socket.on('connect', ()=>{
    console.log('connect success: ' + socket.id);
});
socket.on('notify', (notify)=>{
    console.log("notify", notify);
    server_notify_process(notify);
});

var backnotify;
function setBackNotify(func) {
    backnotify = func;
}

async function createUser(udn, username, password) {
    var response = await requestToServer('createUser', {udn, username, password});
    return response;
}

async function login(_udn, _password) {
    var response = await requestToServer('login', {udn:_udn, password:_password});
    if(response && response.failure == 0) {
        udn = _udn;
        password = _password;
        console.log('%s login success', udn);
        heartbeat();
    }
    return response;
}

async function logout() {
    var response = await requestToServer('logout', {udn, password});
    if(response && response.failure == 0) {
        console.log('%s logout success', udn);
        if(heartTimer) {
            clearInterval(heartTimer);
            heartTimer = null;
            heartCount = -1;
        }
        udn = null;
        password = null;
    }
    return response;
}

var heartCount = -1;
var heartTimer = null;
function heartbeat() {
    heartCount = 5;
    heartTimer = setInterval(()=>{
        requestToServer('heartbeat', {udn, password}).then((response)=>{
            if(response === 'timeout' || response.failure != 0) {
                heartCount--;
                console.log('Heartbeat Fail');
                if(heartCount === 0) {
                    console.log('Network Exception');
                    clearInterval(heartTimer);
                    heartTimer = null;
                    heartCount = -1;
                }
            }
        });
    }, 3000);
}

async function createRoom(roomId, kind) {
    var response = await requestToServer('createRoom', {udn, roomId, kind});
    if(response && response.failure == 0) {
        console.log('createRoom %s success', roomId);
    }
    return response;
}

async function closeRoom(roomId) {
    var response = await requestToServer('closeRoom', {udn, roomId});
    if(response && response.failure == 0) {
        console.log('%s close room %s success', udn, roomId);
        sendTransport.close();
        sendTransport = null;
        recvTransport.close();
        recvTransport = null;
        producers.clear();
        consumers.clear();
    }
    return response;
}

async function join(roomId) {
    var response = await requestToServer('join', {udn, roomId});
    if(response && response.failure == 0) {
        console.log('%s join room %s success', udn, roomId);
    }
    return response;
}

async function leave(roomId) {
    var response = await requestToServer('leave', {udn, roomId});
    if(response && response.failure == 0) {
        console.log('udn[%s] leave room[%s] success', udn, roomId);
        sendTransport.close();
        sendTransport = null;
        recvTransport.close();
        recvTransport = null;
        producers.clear();
        consumers.clear();
    }
    return response;
}

async function getRouterRtpCapabilities(roomId) {
    var response = await requestToServer('getRouterRtpCapabilities', {roomId});
    if(response && response.failure == 0) {
        const routerRtpCapabilities = response.data.routerRtpCapabilities;
        console.log('routerRtpCapabilities:%o', routerRtpCapabilities);
        if(!device.loaded) {
            await device.load({routerRtpCapabilities});
        }
    }
}

async function updateRtpCapabilities() {
    if(device.rtpCapabilities) {
        await requestToServer('updateRtpCapabilities', {udn, rtpCapabilities: device.rtpCapabilities});
    }
}

async function createSendTransport(roomId) {
    var response = await requestToServer('createWebRtcTransport', {udn, roomId, direction:'send', sctpCapabilities:device.sctpCapabilities});
    if(response && response.failure == 0) {
        const {id, iceParameters, iceCandidates, dtlsParameters, sctpParameters} = response.data.transport;
        sendTransport = device.createSendTransport({id, iceParameters, iceCandidates, dtlsParameters, sctpParameters});
        sendTransport.on("connect", async ({ dtlsParameters }, callback, errback)=>{
            console.log('connect callback, the dtlsParameters is', dtlsParameters);
            try {
                var data = {
                    roomId         : roomId,
                    transportId    : sendTransport.id,
                    dtlsParameters : dtlsParameters
                };
                var response = await requestToServer("connect", data);
                if(response && response.failure == 0) {
                    console.log('connect success');
                    callback();
                }
            } catch (error) {
                errback(error);
            }
        });
        sendTransport.on("produce", async (parameters, callback, errback)=>{
            console.log('produce callback, the parameters is', parameters);
            try {
                var data = {
                    roomId        : roomId,
                    transportId   : sendTransport.id, 
                    kind          : parameters.kind,
                    rtpParameters : parameters.rtpParameters,
                    appData       : parameters.appData
                };
                var response = await requestToServer("produce", data);
                if(response && response.failure == 0) {
                    var id = response.data.producerId;
                    callback({id});
                }
            } catch (error) {
                errback(error);
            }
        });
        sendTransport.on("producedata", async (parameters, callback, errback)=>{
            console.log('producedata callback, the parameters is', parameters);
            try {
                var data = {
                    roomId                  : roomId,
                    transportId             : sendTransport.id,
                    sctpStreamParameters    : parameters.sctpStreamParameters,
                    label                   : parameters.label,
                    protocol                : parameters.protocol,
                    appData                 : parameters.appData
                };
                var response = await requestToServer("producedata", data);
                if(response && response.failure == 0) {
                    var id = response.data.producerId;
                    callback({id});
                }
            } catch (error) {
                errback(error);
            }
        });
    }
}

async function createRecvTransport(roomId) {
    var response = await requestToServer('createWebRtcTransport', {udn, roomId, direction:'recv', sctpCapabilities:device.sctpCapabilities});
    if(response && response.failure == 0) {
        const {id, iceParameters, iceCandidates, dtlsParameters, sctpParameters} = response.data.transport;
        recvTransport = device.createRecvTransport({id, iceParameters, iceCandidates, dtlsParameters, sctpParameters});
        recvTransport.on('connect', async ({ dtlsParameters }, callback, errback) =>{
            console.log('connect callback, the dtlsParameters is', dtlsParameters);
            try {
                var data = {
                    roomId         : roomId,
                    transportId    : recvTransport.id,
                    dtlsParameters : dtlsParameters
                };
                var response = await requestToServer("connect", data);
                if(response && response.failure == 0) {
                    console.log('connect success');
                    callback();
                } else {
                    console.log('connect fail');
                    errback(error);
                }
            } catch (error) {
                errback(error);
            }
        });
    }
}

async function webcamProduce(roomId, deviceId) {
    var response = await isAudioOrVideoAllowed(roomId, 'video');
    if(!(response && response.failure == 0 && response.data.result == true)) {
        return false;
    }
    const constrains = {
        video: {
            width: 320,
            height: 240,
            frameRate: 30,
            facingMode: 'enviroment'
        }
    }
    if(deviceId) {
        constrains.video.deviceId = deviceId;
    }
    const stream = await navigator.mediaDevices.getUserMedia(constrains);
    const videoTrack = stream.getVideoTracks()[0];
    var webcamProducer = getProducerByKind('video');
    if(webcamProducer) {
        await webcamProducer.replaceTrack({track: videoTrack});
    } else {
        webcamProducer = await sendTransport.produce({
            track       : videoTrack,
            encodings   :
            [
                { maxBitrate: 100000 }
            ],
            codecOptions :
            {
                videoGoogleStartBitrate : 1000
            }
        });
        producers.set(webcamProducer.id, webcamProducer);
        console.log('webcam producer.id:%s', webcamProducer.id);
    }
    var notify = {
        type: 'newtrack',
        data: {udn, track:videoTrack}
    }
    backnotify(notify);
    return true;
}

async function screenProduce() {
    const stream = await navigator.mediaDevices.getDisplayMedia();
    const screenTrack = stream.getVideoTracks()[0];
    const screenProducer = await sendTransport.produce({
        track       : screenTrack,
        encodings   :
        [
            { maxBitrate: 100000 }
        ],
        codecOptions :
        {
            videoGoogleStartBitrate : 1000
        },
        appData      : {type: 'screen'}
    });
    producers.set(screenProducer.id, screenProducer);
    console.log('screen producer.id:%s', screenProducer.id);
    var notify = {
        type: 'newtrack',
        data: {udn, tracktype:'screen', track:screenTrack}
    }
    backnotify(notify);
}

async function micProduce(roomId, deviceId) {
    var response = await isAudioOrVideoAllowed(roomId, 'audio');
    if(!(response && response.failure == 0 && response.data.result == true)) {
        return false;
    }
    const constrains = {
        audio : true
    };
    if(deviceId) {
        constrains.audio = {deviceId};
    }
    const stream = await navigator.mediaDevices.getUserMedia(constrains);
    const audioTrack = stream.getAudioTracks()[0];
    var micProducer = getProducerByKind('audio');
    if(micProducer) {
        await micProducer.replaceTrack({track: audioTrack});
    } else {
        micProducer = await sendTransport.produce({
            track       : audioTrack,
            encodings   :
            [
                { maxBitrate: 100000 }
            ],
            codecOptions :
            {
                videoGoogleStartBitrate : 1000
            }
        });
        producers.set(micProducer.id, micProducer);
        console.log('mic producer.id:%s', micProducer.id);
    }
    return true;
}

async function getOtherMediasInRoom(roomId) {
    var response = await requestToServer('getOtherMediasInRoom', {udn, roomId});
    return response;
}

//type:pause,resume,stop
//kind:video,audio
async function setMediaStream(roomId, type, kind) {
    var response = await requestToServer('setMediaStream', {udn, roomId, type, kind});
    if(response && response.failure == 0) {
        var producer = getProducerByKind(kind);
        if(!producer) {
            response.failure = 100;
            return response;
        }
        if(type == 'pause') {
            producer.pause();
        } else if(type == 'resume') {
            producer.resume();
        } else if(type == 'stop') {
            producers.delete(producer.id);
            producer.close();
            producer = null;
        }
    }
    return response;
}

async function limitOtherUser(roomId, otherUdn, type, kind) {
    var producerId = getOtherProducerIdByUdnAndKind(otherUdn, kind);
    var response = await requestToServer('limitOtherUser', {roomId, udn, otherUdn, type, kind, producerId});
    return response;
}

async function isAudioOrVideoAllowed(roomId, kind) {
    var response = await requestToServer('isAudioOrVideoAllowed', {roomId, udn, kind});
    return response;
}

async function preSendToUser(remote_udn) {
    if(!getDataProducerByRemoteUdn(remote_udn)) {
        const producer = await sendTransport.produceData({'label': 'chat', appData:{remote_udn}});
        producer.appData.remote_udn = remote_udn;
        dataproducers.set(producer.id, producer);
    }
}

async function sendToUser(remote_udn, data) {
    var dataProducer = getDataProducerByRemoteUdn(remote_udn);
    dataProducer.send(data);
}

//kind:audio,video,screen
function getProducerByKind(kind) {
    if(kind === 'audio') {
        for(let producer of producers.values()) {
            if(producer.kind === 'audio') {
                return producer;
            }
        }
    } else if(kind === 'video') {
        for(let producer of producers.values()) {
            if(producer.kind === 'video' && producer.appData.type != 'screen') {
                return producer;
            }
        }
    } else if(kind === 'screen') {
        for(let producer of producers.values()) {
            if(producer.kind === 'video' && producer.appData.type === 'screen') {
                return producer;
            }
        }
    }
    return null;
}

function getDataProducerByRemoteUdn(udn) {
    for(let dataProducer of dataproducers.values()) {
        if(dataProducer.appData.remote_udn === udn) {
            return dataProducer;
        }
    }
    return null;
}

function getOtherProducerIdByUdnAndKind(udn, kind) {
    for(let consumer of consumers.values()) {
        if(consumer.appData.producerUdn === udn && consumer.kind === kind) {
            return consumer.producerId;
        }
    }
    return null;
}

function server_notify_process(notify) {
    switch(notify.type) {
        case 'consume':
            consume(notify);
            break;
        case 'joined':
            backnotify(notify);
            break;
        case 'otherjoined':
            backnotify(notify);
            break;
        case 'otherleaved':
            backnotify(notify);
            break;
        case 'roomclosed':
            sendTransport.close();
            sendTransport = null;
            recvTransport.close();
            recvTransport = null;
            producers.clear();
            consumers.clear();
            backnotify(notify);
            break;
        case 'trackstop':
            var {consumerId} = notify.data;
            var consumer = consumers.get(consumerId);
            if(consumer) {
                consumers.delete(consumer.id);
                consumer.close();
                consumer = null;
            }
            break;
        case 'limited':
            var {producerId} = notify.data;
            var producer = producers.get(producerId);
            if(producer) {
                producers.delete(producer.id);
                producer.close();
                producer = null;
            }
            backnotify(notify);
            break;
        case 'consumedata':
            consumeData(notify);
            break;
        default:
            break;
    }
}

async function consume(notify) {
    var {udn, id, producerId, kind, rtpParameters, tracktype} = notify.data;
    const consumer = await recvTransport.consume({id, producerId, kind, rtpParameters});
    consumer.appData.producerUdn = udn;
    consumers.set(consumer.id, consumer);
    const {track} = consumer;
    var notify = {
        type: 'newtrack',
        data: {udn, tracktype, track}
    }
    backnotify(notify);
}

async function consumeData(notify) {
    var {udn, id, dataProducerId, sctpStreamParameters, label, protocol} = notify.data;
    const dataConsumer = await recvTransport.consumeData({id, dataProducerId, sctpStreamParameters, label, protocol});
    dataConsumer.appData.producerUdn = udn;
    dataconsumers.set(dataConsumer.id, dataConsumer);
    dataConsumer.on('message', (data)=>{
        console.log(data);
    });
}

global.setBackNotify = setBackNotify;
global.createUser = createUser;
global.login = login;
global.logout = logout;
global.createRoom = createRoom;
global.closeRoom = closeRoom;
global.join = join;
global.leave = leave;
global.getRouterRtpCapabilities = getRouterRtpCapabilities;
global.createSendTransport = createSendTransport;
global.createRecvTransport = createRecvTransport;
global.webcamProduce = webcamProduce;
global.micProduce = micProduce;
global.screenProduce = screenProduce;
global.preSendToUser = preSendToUser;
global.sendToUser = sendToUser;
global.updateRtpCapabilities = updateRtpCapabilities;
global.getOtherMediasInRoom = getOtherMediasInRoom;
global.setMediaStream = setMediaStream;
global.limitOtherUser = limitOtherUser;