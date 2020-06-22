//第一种执行shell命令的方法
require("shelljs/global");
exec("ls", function(status, output)
{
    console.log(output);
});

//第二种执行shell命令的方法
var process = require("child_process");
process.exec("ls", function(error, stdout, stderr){
    console.log(stdout);
});