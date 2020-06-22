/**
 * log4js版本为0.6.6
 * npm安装特定版本的包的命令如下
 * npm install log4js@0.0.6
 */

 /**
  * 
  * appenders：数组对象，每一个元素都是一个appender
  * 
  * type：日志输出类型，主要包括：console-控制台输出、dateFile-根据日期配置自动生成当前日期下的日志、file-纯粹在一个文件中输出
  * 
  * category：日志类别
  * 
  * pattern：日志输出格式
  * 
  * alwaysIncludePattern：是否始终以pattern指定格式输出日志
  * 
  * level：日志输出级别
  * 
  * replaceConsole：控制台自身的打印是否以log4js格式在控制台输出
  * 
  */

var log4js = require('log4js');

var config = {
    appenders: [
        {
            type : 'file',
            filename : __dirname + '/ui.log',
            maxLogSize : 10485760,
            backups : 20,
            category : 'access'
        },
        {
            type : 'console',
            category : 'console'
        }
    ],

    replaceConsole : false
};

log4js.configure(config);

// var logger = log4js.getLogger('console');
var logger = log4js.getLogger('access');

logger.info("test logger");
logger.debug("test hello");

console.log("hello world");