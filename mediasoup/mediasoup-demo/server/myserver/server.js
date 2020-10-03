const os = require('os');
const mediasoup = require('mediasoup');
const https = require('https');
const socketio = require('socket.io');
const fs = require('fs');
const {Room, User} = require('./Room');

var FailureCode = {
    Success : 0,
    UserExists : 1,
    RoomExists : 2,
    CreateUserFailed : 3,
    CreateRoomFailed : 4,
    IllegalUser : 5,
    UserPasswordError : 6,
    RoomNotExists : 7,
    UserNotExists : 8,
    UdnHaveInRoom : 9,
    UdnNotInRoom : 10,
    ConnectError : 11,
    ConsumeError : 12,
    ProduceError : 13,
    StateError : 14,
    UnknowError : 15
}

const mediasoupWorkers = [];
var nextMediasoupWorkerIdx = 0;

async function runMediasoupWorkers() {
    const numWorkers = Object.keys(os.cpus()).length;
	console.log('running %d mediasoup Workers...', numWorkers);
    for(let i = 0; i < numWorkers; i++) {
        const worker = await mediasoup.createWorker({
            logLevel : 'warn',
            dtlsCertificateFile: '../certs/fullchain.pem',
            dtlsPrivateKeyFile: '../certs/privkey.pem'
        });
        mediasoupWorkers.push(worker);
        console.log('create a new worker, pid:%d', worker.pid);
        worker.on('died', ()=>{
            console.log('mediasoup worker died, exiting in 2 seconds,[pid:%d]', worker.pid);
            setTimeout(()=>{process.exit(1)}, 2000);
        });
        setInterval(async() => {
            const usage = await worker.getResourceUsage();
            console.log('mediasoup Worker resource usage [pid:%d]: %o', worker.pid, usage);
        }, 120000);
    }
}

function getMediasoupWorker() {
	const worker = mediasoupWorkers[nextMediasoupWorkerIdx];
	if (++nextMediasoupWorkerIdx === mediasoupWorkers.length)
		nextMediasoupWorkerIdx = 0;
	return worker;
}

function runSignallingServer(request_process) {
    var options = {
        key : fs.readFileSync('../../../../webrtc/server/cert/2_lishifu.work.key'),
        cert: fs.readFileSync('../../../../webrtc/server/cert/1_lishifu.work_bundle.crt')
    }
    var https_server = https.createServer(options);
    https_server.listen(4431, '0.0.0.0');
    //使用https服务器启动一个socketio服务
    var io = socketio.listen(https_server);
    console.log('running signalling server...');
    io.sockets.on('connection', (socket)=> {
        console.log("a new connection arrive : " + socket.id);
        socket.on('request', async (request)=>{
            if(request.type != 'heartbeat') {
                console.log("receive request:", request);
            }
            if(request.type == 'login') {
                var notify = (notify)=>{
                    console.log('send notify:', notify);
                    socket.emit('notify', notify);
                }
                request.data.notify = notify;
            }
            var response = await request_process(request);
            if(response.type != 'heartbeat' || (response.type == 'heartbeat' && response.failure != 0)) {
                console.log('send response:', response);
            }
            socket.emit('response', response);
        });
    });
}

async function run() {
    await runMediasoupWorkers();
    await runSignallingServer(request_process);
}

async function request_process(request) {
    var {taskid, type, data} = request;
    var response = {
        taskid,
        type
    }
    switch(type){
        case 'createUser':
            var {udn, username, password} = data;
            if(User.contains(udn)) {
                response.failure = FailureCode.UserExists;
            } else {
                var user = User.create(udn, username, password);
                if(user) {
                    user.on('create', ()=>{
                        console.log('create user success');
                    });
                    response.failure = FailureCode.Success;
                } else {
                    response.failure = FailureCode.CreateUserFailed;
                }
            }
            break;
        case 'login':
            var {udn, password, notify} = data;
            var user = User.getUserByUdn(udn);
            if(user) {
                if(user.login(password, notify)) {
                    response.failure = FailureCode.Success;
                    response.data = {};
                    response.data.udn = user._udn;
                    response.data.username = user._username;
                } else {
                    response.failure = FailureCode.UnknowError;
                }
            } else {
                response.failure = FailureCode.IllegalUser;
            }
            break;
        case 'logout':
            var {udn, password} = data;
            var user = User.getUserByUdn(udn);
            if(user) {
                if(user.logout(password)) {
                    response.failure = FailureCode.Success;
                } else {
                    response.failure = FailureCode.UnknowError;
                }
            } else {
                response.failure = FailureCode.IllegalUser;
            }
            break;
        case 'heartbeat':
            var {udn, password} = data;
            var user = User.getUserByUdn(udn);
            if(user) {
                user.heartbeat();
                response.failure = FailureCode.Success;
            } else {
                response.failure = FailureCode.IllegalUser;
            }
            break;
        case 'createRoom':
            var {udn, roomId, kind} = data;
            const mediasoupWorker = getMediasoupWorker();
            var room = await Room.create({ mediasoupWorker, udn, roomId, kind});
            if(room) {
                response.failure = FailureCode.Success;
            } else {
                response.failure = FailureCode.UnknowError;
            }
            break;
        case 'closeRoom':
                var {udn, roomId} = data;
                var room = Room.getRoomById(roomId);
                if(room) {
                    if(room.close(udn)) {
                        response.failure = FailureCode.Success;
                    } else {
                        response.failure = FailureCode.UnknowError;
                    }
                } else {
                    response.failure = FailureCode.RoomNotExists;
                }
            break;
        case 'join':
            var {udn, roomId} = data;
            var room = Room.getRoomById(roomId);
            if(room) {
                if(room.join(udn)) {
                    response.failure = FailureCode.Success;
                } else {
                    response.failure = FailureCode.UnknowError;
                }
            } else {
                response.failure = FailureCode.RoomNotExists;
            }
            break;
        case 'leave':
            var {udn, roomId} = data;
            var room = Room.getRoomById(roomId);
            if(room) {
                if(room.leave(udn)) {
                    response.failure = FailureCode.Success;
                } else {
                    response.failure = FailureCode.UnknowError;
                }
            } else {
                response.failure = FailureCode.RoomNotExists;
            }
            break;
        case 'getRouterRtpCapabilities':
            var {roomId} = data;
            var room = Room.getRoomById(roomId);
            if(room) {
                var routerRtpCapabilities = room.getRouterRtpCapabilities();
                response.data = {};
                response.data.routerRtpCapabilities = routerRtpCapabilities;
                response.failure = FailureCode.Success;
            } else {
                response.failure = FailureCode.RoomNotExists;
            }
            break;
        case 'createWebRtcTransport':
            var {udn, roomId, direction, sctpCapabilities} = data;
            var room = Room.getRoomById(roomId);
            if(room) {
                const transport = await room.createWebRtcTransport(udn, direction, sctpCapabilities);
                response.failure = FailureCode.Success;
                response.data = {};
                response.data.transport = transport;
            } else {
                response.failure = FailureCode.RoomNotExists;
            }
        case 'connect':
            var {roomId, transportId, dtlsParameters} = data;
            var room = Room.getRoomById(roomId);
            if(room) {
                room.connect(transportId, dtlsParameters)
                response.failure = FailureCode.Success;
            } else {
                response.failure = FailureCode.RoomNotExists;
            }
            break;
        case 'produce':
            var {roomId, transportId, kind, rtpParameters, appData} = data;
            var room = Room.getRoomById(roomId);
            if(room) {
                var producerId = await room.produce(transportId, kind, rtpParameters, appData);
                if(producerId) {
                    response.failure = FailureCode.Success;
                    response.data = {};
                    response.data.producerId = producerId;
                    console.log('producerId', producerId);
                } else {
                    response.failure = FailureCode.ProduceError;
                }
            } else {
                response.failure = FailureCode.RoomNotExists;
            }
            break;
        case 'producedata':
            var {roomId, transportId, sctpStreamParameters, label, protocol, appData} = data;
            var room = Room.getRoomById(roomId);
            if(room) {
                var producerId = await room.produceData(transportId, sctpStreamParameters, label, protocol);
                if(producerId) {
                    response.failure = FailureCode.Success;
                    response.data = {};
                    response.data.producerId = producerId;
                } else {
                    response.failure = FailureCode.UnknowError;
                }
            } else {
                response.failure = FailureCode.RoomNotExists;
            }
            break;
        case 'consume':
            var {roomId, transportId, producerId, rtpCapabilities} = data;
            var room = Room.getRoomById(roomId);
            if(room) {
                var result = room.consume(transportId, producerId, rtpCapabilities);
                if(result) {
                    response.failure = FailureCode.Success;
                } else {
                    response.failure = FailureCode.ConsumeError;
                }
            } else {
                response.failure = FailureCode.RoomNotExists;
            }
            break;
        case 'updateRtpCapabilities':
            var {udn, rtpCapabilities} = data;
            var user = User.getUserByUdn(udn);
            if(user) {
                user.updateRtpCapabilities(rtpCapabilities);
                response.failure = FailureCode.Success;
            } else {
                response.failure = FailureCode.IllegalUser;
            }
            break;
        case 'getOtherMediasInRoom':
            var {udn, roomId} = data;
            var room = Room.getRoomById(roomId);
            if(room) {
                room.getOtherMediasInRoom(udn);
                response.failure = FailureCode.Success;
            } else {
                response.failure = FailureCode.RoomNotExists;
            }
            break;
        case 'setMediaStream':
            var {udn, roomId, type, kind} = data;
            var room = Room.getRoomById(roomId);
            if(room) {
                var result = await room.setMediaStream(udn, type, kind);
                if(result) {
                    response.failure = FailureCode.Success;
                } else {
                    response.failure = FailureCode.UnknowError;
                }
            } else {
                response.failure = FailureCode.RoomNotExists;
            }
            break;
        case 'limitOtherUser':
            var {roomId, udn, otherUdn, type, kind, producerId} = data;
            var room = Room.getRoomById(roomId);
            if(room) {
                if(room.limitOtherUser(udn, otherUdn, type, kind, producerId)) {
                    response.failure = FailureCode.Success;
                } else {
                    response.failure = FailureCode.UnknowError;
                }
            } else {
                response.failure = FailureCode.RoomNotExists;
            }
            break;
        case 'isAudioOrVideoAllowed':
            var {roomId, udn, kind} = data;
            var room = Room.getRoomById(roomId);
            if(room) {
                if(room.isAudioOrVideoAllowed(udn, kind)) {
                    response.failure = FailureCode.Success;
                    response.data = {};
                    response.data.result = true;
                } else {
                    response.failure = FailureCode.Success;
                    response.data = {};
                    response.data.result = false;
                }
            } else {
                response.failure = FailureCode.RoomNotExists;
            }
            break;
        default:
            break;
    }
    return response;
}

async function test() {
    await run();
    // var udn = '5001';
    // var username = 'lzxysf';
    // var password = '216857';
    // var roomId = 'roomtest';
    // var request = {
    //     task_id : 1,
    //     request_type : 'createUser',
    //     data : {udn, username, password}
    // }
    // var response = request_process(request);
    // console.log(response);

    // var request = {
    //     task_id : 2,
    //     request_type : 'login',
    //     data : {udn, password}
    // }
    // var response = request_process(request);
    // console.log(response);

    // var request = {
    //     task_id : 3,
    //     request_type : 'createRoom',
    //     data : {udn, roomId}
    // }
    // var response = await request_process(request);
    // console.log(response);

    // var request = {
    //     task_id : 4,
    //     request_type : 'getRouterRtpCapabilities',
    //     data : {roomId}
    // }
    // var response = request_process(request);
    // console.log(response);

    // var request = {
    //     task_id : 5,
    //     request_type : 'createWebRtcTransport',
    //     data : {udn, roomId}
    // }
    // var response = await request_process(request);
    // console.log(response);

    // var request = {
    //     task_id : 6,
    //     request_type : 'createWebRtcTransport',
    //     data : {udn, roomId}
    // }
    // var response = await request_process(request);
    // console.log(response);
}

test();