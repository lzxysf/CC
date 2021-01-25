/**
 * WebGL——本例演示varying的使用
 * attribute 用在顶点着色器中接收顶点相关信息
 * uniform 可以同时在顶点着色器和片元着色器中使用，接收与顶点无关的数据
 * varying 同时在顶点着色器(vertexShader)(vs)和片元着色器(fragmentShader)(fs)中定义，用于在两者之间传递数据
 */

// 顶点着色器源码
var vertexShaderSrc = `
attribute vec4 a_Position;
attribute vec4 a_Color;
varying vec4 v_Color;
void main(){
    gl_Position = a_Position;
    v_Color = a_Color;
}`;

// 片段着色器源码
var fragmentShaderSrc = `
precision mediump float;
varying vec4 v_Color;
void main(){
    gl_FragColor = v_Color;
}`;

// 初始化使用的shader
function initShader(gl) {
    var vertexShader = gl.createShader(gl.VERTEX_SHADER);// 创建顶点着色器
    gl.shaderSource(vertexShader, vertexShaderSrc);// 绑定顶点着色器源码
    gl.compileShader(vertexShader);// 编译定点着色器

    var fragmentShader = gl.createShader(gl.FRAGMENT_SHADER);// 创建片段着色器
    gl.shaderSource(fragmentShader, fragmentShaderSrc);// 绑定片段着色器源码
    gl.compileShader(fragmentShader);// 编译片段着色器

    var shaderProgram = gl.createProgram();// 创建着色器程序
    gl.attachShader(shaderProgram, vertexShader);// 指定顶点着色器
    gl.attachShader(shaderProgram, fragmentShader);// 指定片段着色色器
    gl.linkProgram(shaderProgram);// 链接程序
    gl.useProgram(shaderProgram);//使用着色器

    return shaderProgram;
}

function main() {
    var canvas = document.getElementById("container");
    var gl = canvas.getContext("webgl") || canvas.getContext("experimental-webgl");
    var shaderProgram = initShader(gl); //初始化着色器
    gl.clearColor(0.0, 0.0, 0.0, 1.0); //指定清空canvas的颜色
    gl.clear(gl.COLOR_BUFFER_BIT); //清空canvas

    //顶点和颜色数组
    //数组中每5个算一组，每组前2个元素表示顶点位置，后3个元素表示rgb颜色
    var vertexsColors = new Float32Array([
        //顶点坐标   rgb颜色
        0.0,0.5,    1.0,0.0,0.0,
        -0.5,-0.5,  0.0,1.0,0.0,
        0.5,-0.5,   0.0,0.0,1.0]);
    var FSIZE = vertexsColors.BYTES_PER_ELEMENT; //一个元素的字节数

    var vertexColorBuffer = gl.createBuffer(); //创建缓冲区对象
    gl.bindBuffer(gl.ARRAY_BUFFER, vertexColorBuffer); //绑定缓冲区对象
    gl.bufferData(gl.ARRAY_BUFFER, vertexsColors, gl.STATIC_DRAW); //将数据写入缓冲区

    var a_Position = gl.getAttribLocation(shaderProgram, "a_Position"); //获取shader中的a_Position变量
    gl.vertexAttribPointer(a_Position, 2, gl.FLOAT, false, FSIZE * 5, 0); //表示vertexsColors数组中5个是一组，每组偏移0个（即从第1个）开始，共计2个
    gl.enableVertexAttribArray(a_Position);

    var a_Color = gl.getAttribLocation(shaderProgram, "a_Color"); //获取shader中的a_Color
    gl.vertexAttribPointer(a_Color, 3, gl.FLOAT, false, FSIZE * 5, FSIZE * 2); //表示vertexsColors数组中5个是一组，每组偏移2个（即从第3个）开始，共有3个
    gl.enableVertexAttribArray(a_Color);

    gl.drawArrays(gl.TRIANGLES, 0, 3);
}
