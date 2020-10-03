const EventEmitter = require('events').EventEmitter;
const config = require('./config');

var rooms = new Map(); //Map<roomId, room>
var users = new Map(); //Map<udn, user>

class Room extends EventEmitter{
    static contains(roomId) {
        return rooms.get(roomId) ? true : false;
    }
    static getRoomById(roomId) {
        return rooms.get(roomId);
    }
    static async create({mediasoupWorker, udn, roomId, kind}) {
        var user = User.getUserByUdn(udn);
        if(!user) {
            return null;
        } else if(!user.isOnline()) {
            return null;
        } else if(Room.contains(roomId)) {
            return null;
        } else {
            const {mediaCodecs} = config.mediasoup.routerOptions;
            const mediasoupRouter = await mediasoupWorker.createRouter({mediaCodecs});
            const audioLevelObserver = await mediasoupRouter.createAudioLevelObserver(
                {
                    maxEntries : 1,
                    threshold  : -80,
                    interval   : 800
                });
            return new Room(mediasoupRouter, audioLevelObserver, udn, roomId, kind);
        }
    }
    constructor(mediasoupRouter, audioLevelObserver, udn, roomId, kind) {
        super();
        this._mediasoupRouter = mediasoupRouter;
        this._audioLevelObserver = audioLevelObserver;
        this._handleAudioLevelObserver();
        this._kind = kind; //kind, 0:loopback, 1:meetingroom
        this._create_udn = udn;
        this._roomId = roomId;
        this._roomusers = [];
        this._transports = new Map(); //Map<transport.id, transport> transport contains producers and consumers
        rooms.set(this._roomId, this);
        this.emit('create');
    }
    _handleAudioLevelObserver() {
        this._audioLevelObserver.on('volumes', (volumes)=>{
            const { producer, volume } = volumes[0];
            // console.log('udn:%s;volume:%s', producer.appData.udn, volume);
        });
    }
    _getBriefRoomUsers() {
        var briefusers = [];
        for(let i = 0; i < this._roomusers.length; i++) {
            let roomuser = this._roomusers[i];
            let briefuser = {udn:roomuser.udn, username:roomuser.username};
            briefusers.push(briefuser);
        }
        return briefusers;
    }
    _getBriefRoomUserByUdn(udn) {
        for(let i = 0; i < this._roomusers.length; i++) {
            if(this._roomusers[i].udn === udn) {
                let roomuser = this._roomusers[i];
                let briefuser = {udn:roomuser.udn, username:roomuser.username};
                return briefuser;
            }
        }
        return null;
    }
    getRoomUserByUdn(udn) {
        for(let i = 0; i < this._roomusers.length; i++) {
            if(this._roomusers[i].udn === udn) {
                return this._roomusers[i];
            }
        }
        return null;
    }
    close(reqUdn) {
        var user = User.getUserByUdn(reqUdn);
        if(!user) {
            return false;
        } else if(!user.isOnline()) {
            return false;
        }
        rooms.delete(this._roomId);
        for(let transport of this._transports.values()) {
            transport.close();
        }
        this._transports.clear();
        this._transports = null;
        this._mediasoupRouter.close();
        this._mediasoupRouter = null;
        this._audioLevelObserver = null;
        this._create_udn = null;
        for(let i = 0; i < this._roomusers.length; i++) {
            let user = User.getUserByUdn(this._roomusers[i].udn);
            if(user) {
                user._belongRoomId = -1;
                user._rtpCapabilities = null;
                if(user._notify && user._udn != reqUdn) {
                    var notify = {
                        type : 'roomclosed',
                        data : {udn:reqUdn}
                    }
                    user._notify(notify);
                }
            }
        }
        this._roomId = null;
        this._roomusers = null;
        this.emit('close');
        return true;
    }
    join(udn) {
        var user = User.getUserByUdn(udn);
        if(!user) {
            return false;
        } else if(!user.isOnline()) {
            return false;
        } else if (this.isUdnInRoom(udn)) {
            return false;
        }
        user._belongRoomId = this._roomId;
        var roomuser = new RoomUser(udn, user._username);
        this._roomusers.push(roomuser);
        for(let i = 0; i < this._roomusers.length; i++) {
            let user = User.getUserByUdn(this._roomusers[i].udn);
            if(user && user._notify) {
                if(user._udn == udn) {
                    let briefusers = this._getBriefRoomUsers();
                    let notify = {
                        type: 'joined',
                        data: {users: briefusers}
                    };
                    user._notify(notify);
                } else {
                    let briefuser = this._getBriefRoomUserByUdn(udn);
                    let notify = {
                        type: 'otherjoined',
                        data: {user: briefuser}
                    }
                    user._notify(notify);
                }
            }
        }
        return true;
    }
    leave(udn) {
        var user = User.getUserByUdn(udn);
        if(!user) {
            return false;
        } else if(!user.isOnline()) {
            return false;
        } else if (!this.isUdnInRoom(udn)) {
            return false;
        }
        user._belongRoomId = -1;
        user._rtpCapabilities = null;
        var toRemove = [];
        for(let transport of this._transports.values()) {
            if(transport.appData.udn === udn) {
                toRemove.push(transport.id);
                transport.close();
                transport = null;
            }
        }
        for(let i = 0; i < toRemove.length; i++) {
            this._transports.delete(toRemove[i]);
        }
        for(let i = 0; i < this._roomusers.length; i++) {
            if(this._roomusers[i].udn === udn) {
                this._roomusers.splice(i, 1);
                break;
            }
        }
        for(let i = 0; i < this._roomusers.length; i++) {
            let user = User.getUserByUdn(this._roomusers[i].udn);
            if(user && user._notify) {
                var notify = {
                    type: 'otherleaved',
                    data: {udn}
                };
                user._notify(notify);
            }
        }
        return true;
    }
    async getOtherMediasInRoom(udn) {
        if(!this.isUdnInRoom(udn)) {
            return false;
        }
        var user = User.getUserByUdn(udn);
        if(!user) {
            return false;            
        }
        if(!user.isOnline()) {
            return false;
        }
        var recvTransport = null;
        for(let transport of this._transports.values()) {
            if(transport.appData.udn === udn && transport.appData.direction === 'recv') {
                recvTransport = transport;
                break;
            }
        }
        if(recvTransport == null) {
            return false;
        }
        for(let transport of this._transports.values()) {
            if(transport.appData.udn != udn && transport.appData.direction === 'send') {
                for(let producer of transport._producers.values()) {
                    let rtpCapabilities = users.get(udn)._rtpCapabilities;
                    var {id, producerId, kind, rtpParameters} = await this.consume(recvTransport.id, producer.id, rtpCapabilities);
                    if(users.get(udn) && users.get(udn)._notify) {
                        var notify = {
                            type : 'consume',
                            data : {udn: transport.appData.udn, id, producerId, kind, rtpParameters, tracktype: producer.appData.type}
                        };
                        users.get(udn)._notify(notify);
                    }
                }
            }
        }
    }
    isUdnInRoom(udn) {
        for(let i = 0; i < this._roomusers.length; i++) {
            if(this._roomusers[i].udn === udn) {
                return true;
            }
        }
        return false;
    }
    getRouterRtpCapabilities() {
        return this._mediasoupRouter.rtpCapabilities;
    }
    async createWebRtcTransport(udn, direction, sctpCapabilities) {
        if(!this.isUdnInRoom(udn)) {
            return null;
        }
        const webRtcTransportOptions = 
        {
            ...config.mediasoup.webRtcTransportOptions,
            enableSctp : Boolean(sctpCapabilities),
			numSctpStreams : (sctpCapabilities || {}).numStreams,
        }
        var transport = await this._mediasoupRouter.createWebRtcTransport(webRtcTransportOptions);
        transport.appData.udn = udn;
        transport.appData.direction = direction;
        if(direction == 'send') {
            transport.observer.on("newproducer", async (producer) =>{
                console.log("new producer created [id:%s]", producer.id);
                switch(this._kind) {
                    case '0':
                        for(let value of this._transports.values()) {
                            if(value.appData.udn === transport.appData.udn && value.appData.direction === 'recv') {
                                var rtpCapabilities = users.get(value.appData.udn)._rtpCapabilities;
                                const {id, producerId, kind, rtpParameters} = await this.consume(value.id, producer.id, rtpCapabilities);
                                if(users.get(value.appData.udn) && users.get(value.appData.udn)._notify) {
                                    var notify = {
                                        type : 'consume',
                                        data : {udn, id, producerId, kind, rtpParameters, tracktype:producer.appData.type}
                                    };
                                    users.get(value.appData.udn)._notify(notify);
                                }
                            }
                        }
                        break;
                    case '1':
                        for(let value of this._transports.values()) {
                            if(value.appData.udn != transport.appData.udn && value.appData.direction === 'recv') {
                                var rtpCapabilities = users.get(value.appData.udn)._rtpCapabilities;
                                const {id, producerId, kind, rtpParameters} = await this.consume(value.id, producer.id, rtpCapabilities);
                                if(users.get(value.appData.udn) && users.get(value.appData.udn)._notify) {
                                    var notify = {
                                        type : 'consume',
                                        data : {udn, id, producerId, kind, rtpParameters, tracktype:producer.appData.type}
                                    };
                                    users.get(value.appData.udn)._notify(notify);
                                }
                            }
                        }
                        break;
                    default:
                        break;
                }
            });
        }
        this._transports.set(transport.id, transport);
        return {
            id             : transport.id,
            iceParameters  : transport.iceParameters,
            iceCandidates  : transport.iceCandidates,
            dtlsParameters : transport.dtlsParameters,
            sctpParameters : transport.sctpParameters
        };
    }
    connect(transportId, dtlsParameters) {
        var transport = this._transports.get(transportId);
        if(transport) {
            transport.connect({dtlsParameters});
            return true;
        } else {
            return false;
        }
    }
    async produce(transportId, kind, rtpParameters, appData) {
        var transport = this._transports.get(transportId);
        if(transport && transport.appData.direction == 'send') {
            const producer = await transport.produce({kind, rtpParameters, appData});
            producer.appData.udn = transport.appData.udn;
            if(producer.kind === 'audio') {
                this._audioLevelObserver.addProducer({producerId: producer.id});
            }
            return producer.id;
        } else {
            return null;
        }
    }
    async produceData(transportId, sctpStreamParameters, label, protocol, remote_udn) {
        var transport = this._transports.get(transportId);
        var udn = transport.appData.udn;
        if(transport && transport.appData.direction == 'send') {
            const dataProducer = await transport.produceData({sctpStreamParameters, label, protocol});
            console.log('new dataProducer created id[%s]', dataProducer.id);
            dataProducer.appData.udn = udn;
            if(remote_udn) {
                var user = User.getUserByUdn(remote_udn);
                if(user && user._notify) {
                    var dataConsumer = this.getADataConsumerOfBDataProducer(remote_udn, udn);
                    if(!dataConsumer) {
                        var transport = this.getTransportByUdnAndDirection(remote_udn, 'recv');
                        dataConsumer = await transport.consumeData({dataProducerId:dataProducer.id});
                        dataConsumer.appData.udn = remote_udn;
                        console.log('new dataConsumer created id[%s]', dataConsumer.id);
                    }
                    var notify = {
                        type: 'consumedata',
                        data: {udn, id: dataConsumer.id, dataProducerId: dataConsumer.dataProducerId, sctpStreamParameters, label, protocol}
                    }
                    user._notify(notify);
                }
            }
            return dataProducer.id;
        } else {
            return null;
        }
    }
    async consume(transportId, producerId, rtpCapabilities) {
        var transport = this._transports.get(transportId);
        if(transport && transport.appData.direction == 'recv') {
            if(this._mediasoupRouter.canConsume({producerId, rtpCapabilities})) {
                const consumer = await transport.consume({producerId, rtpCapabilities});
                consumer.appData.udn = transport.appData.udn;
                consumer.on('producerclose', ()=>{
                    var udn = consumer.appData.udn;
                    var user = User.getUserByUdn(udn);
                    if(user && user._notify) {
                        var notify = {
                            type: 'trackstop',
                            data: {consumerId: consumer.id}
                        };
                        user._notify(notify);
                    }
                });
                return consumer;
            } else {
                return false;
            }
        } else {
            return false;
        }
    }
    async setMediaStream(udn, type, kind) {
        console.log("udn:%s,type:%s,kind:%s", udn, type, kind);
        if(!this.isUdnInRoom(udn)) {
            console.log('udn %s is not in room', udn);
            return false;
        }
        var transport = this.getTransportByUdnAndDirection(udn, 'send');
        if(!transport) {
            console.log('transport is not found');
            return false;
        }
        var producer = this.getProducerFromTransportByKind(transport, kind);
        if(!producer) {
            console.log('producer is not found');
            return false;
        }
        if(type == 'pause') {
            await producer.pause();
        } else if(type == 'resume') {
            await producer.resume();
        } else if(type == 'stop') {
            await producer.close();
        }
        return true;
    }
    async limitOtherUser(udn, otherUdn, type, kind, producerId) {
        if(!(this.isUdnInRoom(udn) && this.isUdnInRoom(otherUdn))) {
            return false;
        }
        var roomUser = this.getRoomUserByUdn(otherUdn);
        if(roomUser) {
            if(type === 'forbid') {
                if(kind === 'audio') {
                    roomUser.allowAudio = false;
                } else if(kind === 'video') {
                    roomUser.allowVideo = false;
                }
            } else if(type === 'allow') {
                if(kind === 'audio') {
                    roomUser.allowAudio = true;
                } else if(kind === 'video') {
                    roomUser.allowVideo = true;
                }
            }
        }
        if(type === 'forbid') {
            if(producerId) {
                var producer = this.getProducerById(producerId);
                if(producer) {
                    await producer.close();
                }
            }
        }
        var otherUser = User.getUserByUdn(otherUdn);
        if(otherUser && otherUser._notify) {
            var notify = {
                type : 'limited',
                data : {udn, type, kind, producerId}
            }
            otherUser._notify(notify);
        }
        return true;
    }
    isAudioOrVideoAllowed(udn, kind) {
        var roomuser = this.getRoomUserByUdn(udn);
        if(kind === 'audio') {
            return roomuser.allowAudio;
        } else if(kind === 'video') {
            return roomuser.allowVideo;
        } else {
            return false;
        }
    }
    getTransportByUdnAndDirection(udn, direction) {
        for(let transport of this._transports.values()) {
            if(transport.appData.udn === udn && transport.appData.direction === direction) {
                return transport;
            }
        }
        return null;
    }
    getProducerFromTransportByKind(transport, kind) {
        for(let producer of transport._producers.values()) {
            if(producer.kind == kind) {
                return producer;
            }
        }
        return null;
    }
    getProducerById(producerId) {
        for(let transport of this._transports.values()) {
            if(transport.appData.direction === 'send') {
                for(let producer of transport._producers.values()) {
                    if(producer.id === producerId) {
                        return producer;
                    }
                }
                for(let dataProducer of transport._dataProducers.values()) {
                    if(dataProducer.id === producerId) {
                        return dataProducer;
                    }
                }
            }
        }
        return null;
    }
    getADataConsumerOfBDataProducer(udn_a, udn_b) {
        for(let transport of this._transports.values()) {
            if(transport.appData.direction === 'recv' && transport.appData.udn === udn_a) {
                for(let dataConsumer of transport._dataConsumers.values()) {
                    let producer = this.getProducerById(dataConsumer.dataProducerId);
                    if(producer.appData.udn === udn_b) {
                        return dataConsumer;
                    }
                }
                return null;
            }
        }
    }
}

class RoomUser {
    constructor(udn, username) {
        this.udn = udn;
        this.username = username;
        this.allowVideo = true;
        this.allowAudio = true;
    }
}

class User extends EventEmitter{
    static contains(udn) {
        return users.get(udn) ? true : false;
    }
    static getUserByUdn(udn) {
        return users.get(udn);
    }
    static create(udn, username, password) {
        if(users.get(udn)) {
            return null;
        }
        if(!udn || !username || !password) {
            return null;
        }
        return new User(udn, username, password);
    }
    constructor(udn, username, password) {
        super();
        this._udn = udn;
        this._username = username;
        this._password = password;
        this._online = false;
        this._heartTimer = null;
        this._heartCount = -1;
        this._belongRoomId = -1;
        this._rtpCapabilities = null;
        users.set(this._udn, this);
        this.emit('create');
    }
    delete(password) {
        if(this._password === password) {
            if(this._online) {
                this.logout();
            }
            users.delete(this._udn);
            this.emit('delete');
        } else {
            return false;
        }
    }
    heartbeat() {
        if(this._online) {
            this._heartCount = 5;
        }
    }
    login(password, notify) {
        if(this._password === password) {
            this._online = true;
            this._belongRoomId = -1;
            this._notify = notify;
            this._heartCount = 5;
            this._heartTimer = setInterval(()=>{
                this._heartCount--;
                if(this._heartCount == 0) {
                    this.logout();
                }
            } ,3000);
            return true;
        } else {
            return false;
        }
    }
    logout(password) {
        if(this._online && this._password === password) {
            var room = Room.getRoomById(this._belongRoomId);
            if(room) {
                room.leave(this._udn);
            }
            this._belongRoomId = -1;
            this._notify = null;
            if(this._heartTimer) {
                clearInterval(this._heartTimer);
                this._heartTimer = null;
            }
            this._heartCount = -1;
            this._online = false;
            this.rtpCapabilities = null;
            return true;
        } else {
            return false;
        }
    }
    changeUsername(newUsername) {
        this._username = newUsername;
        return true;
    }
    changePassword(newPassword) {
        this.password = newPassword;
        return true;
    }
    isOnline() {
        return this._online;
    }
    updateRtpCapabilities(rtpCapabilities) {
        this._rtpCapabilities = rtpCapabilities;
    }
}

module.exports = {
    Room,
    User
}
