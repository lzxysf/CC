/*
setInterval(cb, ms) 全局函数在指定的毫秒(ms)数后执行指定函数(cb)。
返回一个代表定时器的句柄值。可以使用 clearInterval(t) 函数来清除定时器。
setInterval() 方法会不停地调用函数，直到 clearInterval() 被调用或窗口被关闭。
*/

/**
 * 本程序是定时连续执行5次函数
 */

var count = 0;

function init()
{
    count++;
    if(count > 5)
    {
        clearInterval(t);
        return;
    }
    console.log("init complete");
}

var t = setInterval(init, 1000);
// clearInterval(t);
