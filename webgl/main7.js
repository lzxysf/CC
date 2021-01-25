/**
 * WebGL——简单变换之平移
 * 平移需要1个变量，即平移向量
 */

// 顶点着色器源码
var vertexShaderSrc = `
attribute vec4 a_Position; //该变量接收传入的位置坐标数值，必须声明为全局
uniform vec4 u_Translation; //平移向量
void main(){
    gl_Position = a_Position + u_Translation; // gl_Position 内置变量，表示点的位置，必须赋值
}`;

// 片段着色器源码
var fragmentShaderSrc = `
void main(){
    gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0); //内存变量，表示片元颜色RGBA，必须赋值
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

    //描述了3个点(0.0,0.5) (-0.5,-0.5) (0.5,-0.5)
    //点用(x,y,z,w)表示，程序只给出了(x,y)，那么z自动填充为0.0，w自动填充为1.0
    var vertexs = new Float32Array([
        0.0,0.5,
        -0.5,-0.5,
        0.5,-0.5]);
    var vertexBuffer = gl.createBuffer(); //创建缓冲区对象
    gl.bindBuffer(gl.ARRAY_BUFFER, vertexBuffer); //绑定缓冲区对象
    gl.bufferData(gl.ARRAY_BUFFER, vertexs, gl.STATIC_DRAW); //将数据写入缓冲区

    var a_Position = gl.getAttribLocation(shaderProgram, "a_Position"); //获取shader中的a_Position变量
    gl.vertexAttribPointer(a_Position, 2, gl.FLOAT, false, 0, 0);
    gl.enableVertexAttribArray(a_Position);

    var u_Translation = gl.getUniformLocation(shaderProgram, "u_Translation"); //获取shader中的u_Translation变量
    gl.uniform4f(u_Translation, 0.5, 0.5, 0, 0); //平移向量

    gl.drawArrays(gl.TRIANGLES, 0, 3);
}
