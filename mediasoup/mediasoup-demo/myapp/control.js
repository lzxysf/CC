class UserCard {
    constructor() {
        this.index = 0;
        this.used = false;
        this.udn = null;
        this.usernamenode = null;
        this.videonode = null;
    }
}

class User {
    constructor(udn, username) {
        this.udn = udn;
        this.username = username;
        this.index = 0;
        this.showstream = null;
        this.screenstream = null;
        this.allowvideo = true;
        this.allowaudio = true;
    }
}

new Vue({
    el: '#app',
    data: {
        item_type : 'room',
        udn : null,
        username : null,
        password : null,
        login_udn : null,
        login_password : null,
        current_username : null,
        current_udn : null,
        current_password : null,
        roomId : null,
        roomtypes: [{
            kind: '0',
            label: 'loopback'
          }, {
            kind: '1',
            label: 'meetingroom'
          }],
        roomkind: '',
        current_room : null,
        join_roomId : null,
        audiochecked: false,
        videochecked: true,
        audiodevices: [],
        videodevices: [],
        audiodevice: null,
        videodevice: null,
        currentaudiodevice: null,
        currentvideodevice: null,
        iswebcamrunning : false,
        ismicrunning : false,
        usercards : [],
        roomusers : [],
        screeningudn : '',
        showusers : false,
        showfiles : false,
        roomfiles : []
    },
    mounted() {
        this.initUserCards();
        var user = {udn:'1001', username:'lzxysf', allowvideo:true, allowaudio:true};
        this.roomusers.push(user);
        var file = {filename:'abc.txt', udn:'1001', progress:'20%'};
        this.roomfiles.push(file);
    },
    methods: {
        changeItemType : function(type) {
            this.item_type = type;
            if(type == 'joinRoom') {
                this.getDevices();
            }
        },
        _register : async function() {
            if(!this.udn || !this.username || !this.password) {
                this.$message("用户名、用户号码、密码不能为空");
                return;
            }
            var response = await createUser(this.udn, this.username, this.password);
            if(response && response.failure == 0) {
                this.$message("注册成功,请登录");
                this.udn = null;
                this.username = null;
                this.password = null;
                this.item_type = 'login';
            } else {
                this.$message("注册失败");
            }
        },
        _login : async function() {
            if(!this.login_udn || !this.login_password) {
                this.$message("用户号码、密码不能为空");
                return;
            }
            if(this.current_udn) {
                this.$message("请先注销账号" + this.current_udn);
                return;
            }
            var response = await login(this.login_udn, this.login_password);
            if(response && response.failure == 0) {
                this.$message("登陆成功");
                setBackNotify(this.backnotify);
                this.current_udn = response.data.udn;
                this.current_username = response.data.username;
                this.current_password = this.login_password;
                this.login_udn = null;
                this.login_password = null;
            } else {
                this.$message("登陆失败");
            }
        },
        _logout : async function() {
            if(this.current_udn == null || this.current_password == null) {
                this.$message("当前未登陆");
                return;
            }
            var response = await logout();
            if(response && response.failure == 0) {
                this.$message("注销成功");
                this.current_username = null;
                this.current_udn = null;
                this.current_password = null;
            } else {
                this.$message("注销失败");
            }
        },
        _createRoom : async function() {
            if(this.current_udn == null) {
                this.$message("请先登录您的账号");
                this.item_type = 'login';
                return;
            }
            if(this.roomId == null || this.roomkind == null) {
                this.$message("房间号码或房间类型不能为空");
                return;
            }
            var response = await createRoom(this.roomId, this.roomkind);
            if(response && response.failure == 0) {
                this.roomId = null;
                this.roomkind = null;
                this.$message('创建房间成功');
            } else {
                this.$message('创建房间失败');
            }
        },
        _joinRoom : async function() {
            this.item_type = 'joinRoom';
            if(!this.join_roomId) {
                this.$message('请输入房间号码');
                return;
            }
            if(this.audiochecked && this.audiodevice == null) {
                this.$message('选择一个音频输入设备');
                return;
            }
            if(this.videochecked && this.videodevice == null) {
                this.$message('选择一个视频输入设备');
                return;
            }
            var response =  await join(this.join_roomId);
            if(response && response.failure == 0) {
                this.$message('加入房间成功');
                this.current_room = this.join_roomId;
                this.join_roomId = null;
                this.item_type = 'room';
                await getRouterRtpCapabilities(this.current_room);
                await updateRtpCapabilities();
                await createSendTransport(this.current_room);
                await createRecvTransport(this.current_room);
                if(this.videochecked && this.videodevice) {
                    if(await webcamProduce(this.current_room, this.videodevice)) {
                        this.currentvideodevice = this.videodevice;
                        this.iswebcamrunning = true;
                    } else {
                        this.$message('操作失败');
                    }
                }
                if(this.audiochecked && this.audiodevice) {
                    if(await micProduce(this.current_room, this.audiodevice)) {
                        this.currentaudiodevice = this.audiodevice;
                        this.ismicrunning = true;
                    } else {
                        this.$message('操作失败');
                    }
                }
                getOtherMediasInRoom(this.current_room);
            } else {
                this.$message('加入房间失败');
            }
        },
        _leaveRoom : function() {
            if(this.current_udn == null) {
                this.$message('当前未登陆');
                return;
            }
            if(this.current_room == null) {
                this.$message('当前未加入房间');
                return;
            }
            this.$confirm('是否确定离开房间?', '提示', {
                confirmButtonText: '确定',
                cancelButtonText: '取消',
                type: 'warning'
                }).then(async () => {
                    var response = await leave(this.current_room);
                    if(response && response.failure == 0) {
                        this.$message('离开房间成功');
                        this.clearAllUserCards();
                        this.delAllUsers();
                        this.current_room = null;
                        this.item_type = 'joinRoom';
                    } else {
                        this.$message('离开房间失败');
                    }
                }).catch(() => {
                });
        },
        _closeRoom : function() {
            if(this.current_udn == null) {
                this.$message('当前未登陆');
                return;
            }
            if(this.current_room == null) {
                this.$message('当前未加入房间');
                return;
            }
            this.$confirm('是否确定关闭房间?', '提示', {
                confirmButtonText: '确定',
                cancelButtonText: '取消',
                type: 'warning'
                }).then(async () => {
                    var response = await closeRoom(this.current_room);
                    if(response && response.failure == 0) {
                        this.$message('关闭房间成功');
                        this.clearAllUserCards();
                        this.delAllUsers();
                        this.current_room = null;
                        this.item_type = 'joinRoom';
                    } else {
                        this.$message('关闭房间失败');
                    }
                }).catch(() => {
                });
        },
        initUserCards : function() {
            var cards = document.getElementsByClassName('user-card');
            for(let i = 0; i < cards.length; i++) {
                let childs = cards[i].children;
                let usercard = new UserCard();
                usercard.index = i + 1;
                for(let j = 0; j < childs.length; j++) {
                    if(childs[j].className == 'diy-user') {
                        usercard.usernamenode = childs[j];
                    } else if(childs[j].className == 'diy-video') {
                        usercard.videonode = childs[j];
                    }
                }
                this.usercards.push(usercard);
            }
        },
        getOneEmptyCard : function() {
            for(let i = 0; i < this.usercards.length; i++) {
                if(this.usercards[i].used === false) {
                    this.usercards[i].used = true;
                    return this.usercards[i];
                }
            }
            return null;
        },
        getUserCardByUdn : function(udn) {
            for(let i = 0; i < this.usercards.length; i++) {
                if(this.usercards[i].udn === udn) {
                    return this.usercards[i];
                }
            }
            return null;
        },
        clearUserCardByUdn : function(udn) {
            for(let i = 0; i < this.usercards.length; i++) {
                if(this.usercards[i].udn === udn) {
                    var usercard = this.usercards[i];
                    usercard.udn = null;
                    usercard.videonode.srcObject = null;
                    usercard.usernamenode.innerHTML = null;
                    usercard.used = false;
                    break;
                }
            }
        },
        clearAllUserCards : function() {
            for(let i = 0; i < this.usercards.length; i++) {
                if(this.usercards[i].used === true) {
                    var usercard = this.usercards[i];
                    usercard.used = false;
                    usercard.udn = null;
                    usercard.videonode.srcObject = null;
                    usercard.usernamenode.innerHTML = null;
                }
            }
        },
        getUserByUdn : function(udn) {
            for(let i = 0; i < this.roomusers.length; i++) {
                if(this.roomusers[i].udn === udn) {
                    return this.roomusers[i];
                }
            }
            return null;
        },
        delUserByUdn : function(udn) {
            for(let i = 0; i < this.roomusers.length; i++) {
                if(this.roomusers[i].udn === udn) {
                    this.roomusers.splice(i, 1);
                    break;
                }
            }
        },
        delAllUsers : function() {
            this.roomusers = [];
        },
        getDevices : async function() {
            this.audiodevices = [];
            this.videodevices = [];
            var devicesinfo = await navigator.mediaDevices.enumerateDevices();
            for(let i = 0; i < devicesinfo.length; i++) {
                var device = devicesinfo[i];
                var item = {value:device.deviceId, label:device.label==''?'未知设备':device.label};
                switch(device.kind){
                    case "audioinput":
                        this.audiodevices.push(item);
                        break;
                    case "videoinput":
                        this.videodevices.push(item);
                        break;
                }
            }
        },
        controlLocalMedia : async function(kind) {
            if(kind == 'video') {
                if(this.iswebcamrunning) {
                    var response = await setMediaStream(this.current_room, 'stop', 'video');
                    if(response && response.failure == 0) {
                        this.iswebcamrunning = false;
                        this.$message('关闭摄像头成功');
                        console.log('关闭摄像头成功');
                    } else {
                        this.$message('关闭摄像头失败');
                        console.log('关闭摄像头失败');
                    }
                } else {
                    if(await webcamProduce(this.current_room, this.currentvideodevice)) {
                        this.iswebcamrunning = true;
                        this.$message('开启摄像头成功');
                    } else {
                        this.$message('操作失败');
                    }
                }
            } else if(kind == 'audio') {
                if(this.ismicrunning) {
                    var response = await setMediaStream(this.current_room, 'stop', 'audio');
                    if(response && response.failure == 0) {
                        this.ismicrunning = false;
                        this.$message('关闭麦克风成功');
                    } else {
                        this.$message('关闭麦克风失败');
                    }
                } else {
                    if(await micProduce(this.current_room, this.currentaudiodevice)) {
                        this.ismicrunning = true;
                        this.$message('开启麦克风成功');
                    } else {
                        this.$message('操作失败');
                    }
                }
            }
        },
        switchCamera : async function() {
            if(this.iswebcamrunning == false) {
                this.$message('请先开启摄像头');
                return;
            }
            if(this.videodevices.length == 1) {
                this.$message('当前只有一个摄像头,无法切换');
                return;
            }
            for(let i = 0; i < this.videodevices.length; i++) {
                if(this.videodevices[i].value != this.videodevice) {
                    if(await webcamProduce(this.current_room, this.videodevices[i].value)) {
                        this.videodevice = this.videodevices[i].value;
                        this.$message('切换摄像头成功');
                    } else {
                        this.$message('操作失败');
                    }
                    break;
                }
            }
        },
        shareScreen : function() {
            screenProduce();
        },
        handleCommand : async function(row, type, kind) {
            var otherUdn = row.udn;
            var username = row.username;
            this.$message('udn is ' + otherUdn + ';username is ' + username);
            if((type === 'forbid' || type === 'allow') && (kind === 'audio' || kind === 'video')) {
                var response = await limitOtherUser(this.current_room, otherUdn, type, kind);
                if(response && response.failure == 0) {
                    var user = this.getUserByUdn(otherUdn);
                    if(type === 'forbid') {
                        if(kind === 'video') {
                            user.allowvideo = false;
                        } else if(kind === 'audio') {
                            user.allowaudio = false;
                        }
                    } else if(type === 'allow') {
                        if(kind === 'video') {
                            user.allowvideo = true;
                        } else if(kind === 'audio') {
                            user.allowaudio = true;
                        }
                    }
                } else {
                    this.$message('用户权限管理失败');
                }
            }
        },
        handleFile : function(row, command) {
            var filename = row.filename;
            if(command === 'delfile') {
                for(let i = 0; i < this.roomfiles.length; i++) {
                    if(this.roomfiles[i].filename === filename) {
                        this.roomfiles.splice(i, 1);
                        this.$message('删除文件成功');
                        break;
                    }
                }
            }
        },
        uploadFile : function() {
            preSendToRoom();
            var uploadInput = document.createElement('input');
            uploadInput.type = 'file';
            uploadInput.click();
            uploadInput.onchange = function() {
                var selectfile = uploadInput.files[0];
                let reader = new FileReader();
                reader.onload = function(e) {
                    var data = e.target.result;
                    console.log(data);
                    sendToRoom(data);
                }
                reader.readAsBinaryString(selectfile);
            }
        },
        backnotify : function(notify) {
            if(notify.type == 'newtrack') {
                var {udn, tracktype, track} = notify.data;
                this.process_newtrack_notify(udn, tracktype, track);
            } else if(notify.type == 'joined') {
                var {users} = notify.data;
                this.process_joined_notify(users);
            } else if(notify.type == 'otherjoined') {
                var {user} = notify.data;
                this.process_otherjoined_notify(user);
            } else if(notify.type == 'otherleaved') {
                var {udn} = notify.data;
                this.process_otherleaved_notify(udn);
            } else if(notify.type == 'roomclosed') {
                var {udn} = notify.data;
                this.process_roomclosed_notify(udn);
            } else if(notify.type == 'limited') {
                var {udn, type, kind} = notify.data;
                if(type === 'forbid') {
                    if(kind === 'video') {
                        this.iswebcamrunning = false;
                    } else if(kind === 'audio') {
                        this.ismicrunning = false;
                    }
                }
                this.$message('主持人'+udn+type+'了你的'+kind);
            }
        },
        process_newtrack_notify : function(udn, tracktype, track) {
            var user = this.getUserByUdn(udn);
            if(!user) {
                return;
            }
            if(tracktype === 'screen') {
                user.screenstream = new MediaStream([track]);
                this.screeningudn = udn;
                document.getElementsByClassName('screen-video')[0].srcObject = user.screenstream;
                this.item_type = 'screen';
            } else {
                if(user.showstream) {
                    var tracks = user.showstream.getTracks();
                    for(let i = 0; i < tracks.length; i++) {
                        if(tracks[i].kind === track.kind) {
                            user.showstream.removeTrack(tracks[i]);
                        }
                    }
                    user.showstream.addTrack(track);
                } else {
                    user.showstream = new MediaStream([track]);
                    var usercard = this.getUserCardByUdn(udn);
                    usercard.videonode.srcObject = user.showstream;
                }
            }
        },
        process_joined_notify : function(users) {
            console.log('房间中的用户:', users);
            for(let i = 0; i < users.length; i++) {
                var user = new User(users[i].udn, users[i].username);
                this.roomusers.push(user);
                var usercard = this.getOneEmptyCard();
                user.index = usercard.index;
                usercard.usernamenode.innerHTML = user.username;
                usercard.udn = user.udn;
            }
        },
        process_otherjoined_notify : function(user) {
            console.log('用户%s加入房间', JSON.stringify(user));
            if(user && user.udn && user.username) {
                var _user = new User(user.udn, user.username);
                this.roomusers.push(_user);
                var usercard = this.getOneEmptyCard();
                user.index = usercard.index;
                usercard.usernamenode.innerHTML = user.username;
                usercard.udn = user.udn;
            }
        },
        process_otherleaved_notify : function(udn) {
            this.delUserByUdn(udn);
            this.clearUserCardByUdn(udn);
        },
        process_roomclosed_notify : function(udn) {
            this.$message('主持人[' + udn + ']关闭了房间');
            this.clearAllUserCards();
            this.delAllUsers();
            this.current_room = null;
            this.item_type = 'joinRoom';
        }
    }
});
