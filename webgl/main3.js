/**
 * 使用WebGL画点
 * 前面的代码将点的位置和大小都直接写在了顶点着色器中，现在将其改为由外面的程序传入。
 */

 /**
 * 向shader中传值有两种方式：
 * attribute变量，传递与顶点相关的数组，只能在顶点着色器中使用，attribute变量的个数有上限，值与设备相关
 * uniform变量，传递与顶点无关的数据，在顶点着色器和片元着色器中都可以使用，数量也有上限，具体多少也是与设备相关
 */

// 顶点着色器源码
var vertexShaderSrc = `
attribute vec4 a_Position; //该变量接收传入的位置坐标数值，必须声明为全局
attribute float a_PointSize; //该变量接收传入的点的大小数值，必须声明为全局
void main(){
    gl_Position = a_Position; // gl_Position 内置变量，表示点的位置，必须赋值
    gl_PointSize = a_PointSize; // gl_PointSize 内置变量，表示点的大小（单位像素），可以不赋值，默认为1.0，绘制单个点时才生效
}`;

// 片段着色器源码
var fragmentShaderSrc = `
void main(){
    gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);// 内存变量，表示片元颜色RGBA
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
    var shaderProgram = initShader(gl);// 初始化着色器
    gl.clearColor(0.0, 0.0, 0.0, 1.0);// 指定清空canvas的颜色
    gl.clear(gl.COLOR_BUFFER_BIT);// 清空canvas

    //获取变量 a_Position 和 a_PointSize 并给变量赋值
    //这两个变量在前面 vertexShaderSrc 中已经声明为全局变量
    var a_Position = gl.getAttribLocation(shaderProgram, "a_Position"); //获取shader中的a_Position变量
    gl.vertexAttrib4f(a_Position, 0.0, 0.0, 0.0, 1.0); //给变量a_Position赋值
    var a_PointSize = gl.getAttribLocation(shaderProgram, "a_PointSize"); //获取shader中的a_PointSize变量
    gl.vertexAttrib1f(a_PointSize, 10.0); //给变量a_PointSize赋值

    gl.drawArrays(gl.POINTS, 0, 1);// 画点
}
