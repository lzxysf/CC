
/*
    json字符串的key应该有双引号括起来，否则解析会报错
*/
var callinfo = '{"udn": "1002", "utype": 1, "remote_udn": "9001", "remote_utype": 2}';

//json字符串➡️json对象 传入的必须是一个字符串
var info = JSON.parse(callinfo);

console.log(info["udn"]); //1002
console.log(info["utype"]); //1
console.log(info["abc"]); //undefined

console.log("------------");
//遍历对象中的每一个元素
for (var key in info)
{
    console.log(key + ":" + info[key]);
}
console.log("-------------");

//json对象➡️json字符串
var str = JSON.stringify(info);
console.log(str);
