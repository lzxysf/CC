/**
 * WebGL——矩阵变换
 * 用矩阵实现图形的平移、旋转、缩放
 */

// 顶点着色器源码
var vertexShaderSrc = `
attribute vec4 a_Position; //该变量接收传入的位置坐标数值，必须声明为全局
uniform mat4 u_Mat; //矩阵，可用于平移旋转缩放等
void main(){
    gl_Position = u_Mat * a_Position;
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

    //平移矩阵，列主序
    // var mat = [
    //     1.0, 0.0, 0.0, 0.0,
    //     0.0, 1.0, 0.0, 0.0,
    //     0.0, 0.0, 1.0, 0.0,
    //     0.5, 0.5, 0.0, 1.0,
    // ];

    //旋转矩阵，列主序
    // var rad = Math.PI * 0.5;
    // var mat = new Float32Array([
    //     Math.cos(rad), Math.sin(rad), 0.0, 0.0,
    //     -Math.sin(rad), Math.cos(rad), 0.0, 0.0,
    //     0.0, 0.0, 1.0, 0.0,
    //     0.0, 0.0, 0.0, 1.0,
    // ]);

     // 缩放矩阵，列主序
     var mat = new Float32Array([
        0.5, 0.0, 0.0, 0.0,
        0.0, 0.5, 0.0, 0.0,
        0.0, 0.0, 1.0, 0.0,
        0.0, 0.0, 0.0, 1.0,
    ]);

    var u_Mat = gl.getUniformLocation(shaderProgram, "u_Mat"); //获取shader中的u_Mat矩阵
    gl.uniformMatrix4fv(u_Mat, false, mat); //向u_Mat传入矩阵值 //第二个参数为transpose，表示是否转置，必须为false

    gl.drawArrays(gl.TRIANGLES, 0, 3);
}
