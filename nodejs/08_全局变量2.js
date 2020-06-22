console.log(__filename);            //文件的绝对路径
console.log(__dirname);             //当前执行脚本所在目录
console.log(process.getgid());      //获取进程组标志
console.log(process.cwd());         //当前进程的工作目录
process.chdir('/');                 //更改当前进程的工作目录为根目录
console.log(process.cwd());         //当前进程的工作目录
