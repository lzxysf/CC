const io = require('socket.io-client')

//经测试io和io.connect都可以
var socket = io('https://lishifu.work:4433');

//socket.io内置事件,成功连接后触发
socket.on('connect', ()=>{
    console.log('connect success: ' + socket.id);
});

var taskid = 0;
function getTaskID() {
    return taskid++;
}

function requestToServer(requestType) {
    return new Promise((resolve, reject)=>{
        let taskid = getTaskID();
        let req_data = {
            type: requestType,
            taskid: taskid
        }
        socket.on('response', res_data=>{
            if(t1 && res_data.type == requestType && res_data.taskid == taskid) {
                clearTimeout(t1);
                resolve(res_data);
            }
        });
        let t1 = setTimeout(() => {
            t1 = null;
            resolve(null);
        }, 1000);
        socket.emit('request', req_data);
    });
}

async function test() {
    var data = await requestToServer('changeUsername');
    console.log(data);
}

test();