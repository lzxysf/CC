1.概述
server.js作为webrtc的demo项目的web服务器和信令服务器
server.js持续改进，其它文件server文件不再更新

server.js web服务器(https:4430) + 信令服务器（https:4430）              持续更新改进
server1.js web服务器(http:3000,https:4430) + 信令服务器（https:4430）
server2.js 信令服务器（https:4430）(需要nginx等作为web服务器)

项目中目前使用server.js作为服务器

2.server.js是信令服务器文件
node server.js & 即可运行，&表示在一个新进程中运行不堵塞当前shell

3.server.js文件的步骤为：
(1)npm init创建package.json文件
(2)使用npm install --save安装express serve-index socket.io
    --save的作用是将安装的模块信息配置进package.json文件dependencies属性中
    express是基于nodejs的web开发框架
    serve-index是一个nodejs模块，功能是将文件夹中文件列表显示到浏览器中，和express配套使用
    socket.io是一个WebSocket库，包括了客户端的js和服务器端的nodejs，它的目标是构建可以在不同浏览器和移动设备上使用的实时应用,信令服务器就是基于它构建的
（3）node server.js & 运行信令服务器

4.公网IP、域名、SSL证书
当前用的公网ip为 140.143.188.52
140.143.188.52服务器对应的域名为lishifu.work，有域名了就可以通过lishifu.work访问这个服务器（注意，根据相关规定，域名需要备案的，否则无法正常使用）
但是要想使用https服务，需要申请SSL证书（有免费的SSL证书可以申请，当前证书时限1年，到2021-07-02）（申请SSL证书必须有一个域名）,
认证SSL证书成功后需要下载文件，对应Nginx、Tomcat、IIS等https服务器有不同认证文件，具体可百度如何配置。

为了保证安全webrtc中调用摄像头麦克风的相关操作，必须是使用https业务，http业务中会提示无相应的函数，当然在本地直接打开网页也可以调用摄像头等。
要想获得https业务，需要 公网IP + 域名 + SSL证书
