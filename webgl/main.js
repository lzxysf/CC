/**
 * WebGL——纹理映射
 * 显示两张图片，一张图片覆盖另一张图片
 */

// 顶点着色器源码
var vertexShaderSrc = `
attribute vec4 a_Position;
attribute vec2 a_TexCoord; //映射到的坐标
varying vec2 v_TexCoord;
void main(){
    gl_Position = a_Position;
    v_TexCoord = a_TexCoord;
}`;

// 片段着色器源码
// vec4 texture2D(sampler2D sampler, vec2 coord)  通过texture2D获取纹理映射到的坐标的颜色
var fragmentShaderSrc = `
precision mediump float;
uniform sampler2D u_Sampler1; //背景纹理
uniform sampler2D u_Sampler2; //前端纹理
varying vec2 v_TexCoord; //映射到的坐标
void main(){
    vec4 color1 = texture2D(u_Sampler1, v_TexCoord);
    vec4 color2 = texture2D(u_Sampler2, v_TexCoord);
    if(color2.a > 0.0)
    {
        gl_FragColor = color2;
    }
    else
    {
        gl_FragColor = color1;
    }
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

    gl.shaderProgram = shaderProgram;
}

function initVertexBuffers(gl)
{
    //顶点坐标和映射坐标
    //数组中每4个算一组，每组前2个元素表示顶点坐标，后2个元素表示映射坐标
    var vertexsColors = new Float32Array([
        //顶点坐标   映射坐标
        -1.0,1.0,    0.0,1.0,
        -1.0,-1.0,   0.0,0.0,
        1.0,1.0,     1.0,1.0,
        1.0,-1.0,    1.0,0.0]);
    var FSIZE = vertexsColors.BYTES_PER_ELEMENT; //一个元素的字节数

    var vertexColorBuffer = gl.createBuffer(); //创建缓冲区对象
    gl.bindBuffer(gl.ARRAY_BUFFER, vertexColorBuffer); //绑定缓冲区对象
    gl.bufferData(gl.ARRAY_BUFFER, vertexsColors, gl.STATIC_DRAW); //将数据写入缓冲区

    var a_Position = gl.getAttribLocation(gl.shaderProgram, "a_Position"); //获取shader中的a_Position变量
    gl.vertexAttribPointer(a_Position, 2, gl.FLOAT, false, FSIZE * 4, 0); //表示vertexsColors数组中5个是一组，每组偏移0个（即从第1个）开始，共计2个
    gl.enableVertexAttribArray(a_Position);

    var a_TexCoord = gl.getAttribLocation(gl.shaderProgram, "a_TexCoord"); //获取shader中的a_TexCoord
    gl.vertexAttribPointer(a_TexCoord, 2, gl.FLOAT, false, FSIZE * 4, FSIZE * 2); //表示vertexsColors数组中4个是一组，每组偏移2个（即从第3个）开始，共计2个
    gl.enableVertexAttribArray(a_TexCoord);
}

var loadedImage1 = false;
var loadedImage2 = false;
function loadTexture(gl, image, sampler, index)
{
    gl.pixelStorei(gl.UNPACK_FLIP_Y_WEBGL, true);// 图像坐标与纹理坐标Y轴方向相反，需进行Y轴反转

    var texture = gl.createTexture(); //创建纹理
    if(index == 0)
    {
        gl.activeTexture(gl.TEXTURE0); //激活0号纹理（0号是默认激活的纹理单元）
    }
    else if(index == 1)
    {
        gl.activeTexture(gl.TEXTURE1); //激活1号纹理
    }
    gl.bindTexture(gl.TEXTURE_2D, texture); //绑定纹理对象到激活的纹理单元
    // 配置纹理参数
    gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MAG_FILTER, gl.LINEAR);// 纹理放大方式
    gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MIN_FILTER, gl.LINEAR);// 纹理缩小方式
    gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_WRAP_S, gl.CLAMP_TO_EDGE);// 纹理水平填充方式
    gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_WRAP_T, gl.CLAMP_TO_EDGE);// 纹理垂直填充方式

    var color = index == 0 ? gl.RGB : gl.RGBA; //前面的图片是一个有透明度的图片
    gl.texImage2D(gl.TEXTURE_2D, 0, color, color, gl.UNSIGNED_BYTE, image);// 配置纹理图像
    gl.uniform1i(sampler, index); //将0号纹理传递给着色器

    if(index == 0)
    {
        loadedImage1 = true;
    }
    else if(index == 1)
    {
        loadedImage2 = true;
    }

    if(loadedImage1 && loadedImage2)
    {
        gl.clearColor(0.0, 0.0, 0.0, 1.0); //指定清空canvas的颜色
        gl.clear(gl.COLOR_BUFFER_BIT); //清空canvas
        gl.drawArrays(gl.TRIANGLE_STRIP, 0, 4);
    }
}

function initTextures(gl)
{
    var u_Sampler1 = gl.getUniformLocation(gl.shaderProgram, "u_Sampler1");
    var u_Sampler2 = gl.getUniformLocation(gl.shaderProgram, "u_Sampler2");
    var image1 = new Image();
    var image2 = new Image();
    image1.onload = function(){
        loadTexture(gl, image1, u_Sampler1, 0);
    }
    image2.onload = function(){
        loadTexture(gl, image2, u_Sampler2, 1);
    }
    image1.src = "./background.jpg"; //加载图片的方法由浏览器提供，是异步的，需要在加载完成之后的回调中进行绘制，即上文的image.onload函数
    image2.src = "./child.png";
}

function main() {
    var canvas = document.getElementById("container");
    var gl = canvas.getContext("webgl") || canvas.getContext("experimental-webgl");
    initShader(gl); //初始化着色器
    initVertexBuffers(gl);
    initTextures(gl);
}
