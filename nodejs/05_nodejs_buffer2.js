/**
 * 
 * Buffer.alloc(size[, fill[, encoding]])： 返回一个指定大小的 Buffer 实例，如果没有设置 fill，则默认填满0
 * Buffer.allocUnsafe(size)： 返回一个指定大小的 Buffer 实例，但是它不会被初始化，所以它可能包含敏感的数据
 * 
 */

const buf = Buffer.alloc(256);
var len = buf.write("hello world!");
console.log("写入数据长度为"+len);
console.log(buf.toString());
