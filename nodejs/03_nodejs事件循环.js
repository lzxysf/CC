//引入events模块
var events = require("events");
//创建eventEmitter模块
var eventEmitter = new events.EventEmitter();

var completedHandler = function(){
    console.log("处理完成");
}

//绑定事件名称和事件处理函数
eventEmitter.on("completed", completedHandler);

//触发事件
eventEmitter.emit("completed");
