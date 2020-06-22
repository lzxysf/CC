//setTimeout(cb,ms)
//定时指定的毫秒ms后，执行回调函数cb，只执行一次

//clearTimeout(t)
//用于停止一个由setTimeout创建的定时器

function init()
{
    console.log("init complete");
}
var t = setTimeout(init, 2000);
// clearTimeout(t);
