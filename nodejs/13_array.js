//length可变
var arr = []
arr.length = 10
arr[2] = 15;
console.log(arr);
arr.length = 3;
console.log(arr);

//index可以超出length
arr[10] = 89;
console.log(arr);

console.log("------------------");
//push,pop 都是在数组末尾插入和弹出 
//push返回插入后数组长度，pop返回弹出的数据
arr = [1,2,3,4,5,6];
arr.push("hello");
console.log(arr);
arr.pop()
console.log(arr);
console.log("------------------");

console.log("------------------");
//unshift,shift 从头部添加和移除
arr.unshift("hello");
console.log(arr);
arr.shift();
console.log(arr);
console.log("------------------");

//反转
arr.reverse();
console.log(arr);

//排序
arr.sort();
console.log(arr);
