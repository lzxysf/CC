var width = 1920;
var height = 1080;
// 顶点着色器源码
var vertexShaderSrc = `
attribute lowp vec4 a_Position;
attribute vec2 a_TexCoord; //映射到的坐标
varying vec2 v_TexCoord;
void main(){
    gl_Position = a_Position;
    v_TexCoord = a_TexCoord;
}`;

// 片段着色器源码
// vec4 texture2D(sampler2D sampler, vec2 coord)  通过texture2D获取纹理映射到的坐标的颜色
var fragmentShaderSrc = `
precision lowp float;
uniform sampler2D u_SamplerY; //Y纹理
uniform sampler2D u_SamplerU; //U纹理
uniform sampler2D u_SamplerV; //V纹理
varying vec2 v_TexCoord; //映射到的坐标
void main(){
    float r,g,b,y,u,v,fYmul;
    y = texture2D(u_SamplerY, v_TexCoord).r;
    u = texture2D(u_SamplerU, v_TexCoord).r;
    v = texture2D(u_SamplerV, v_TexCoord).r;

    fYmul = y * 1.1643828125;
    r = fYmul + 1.59602734375 * v - 0.870787598;
    g = fYmul - 0.39176171875 * u - 0.81296875 * v + 0.52959375;
    b = fYmul + 2.01723046875 * u - 1.081389160375;
    gl_FragColor = vec4(r, g, b, 1.0);
}`;

// 初始化使用的shader
function initShader() {
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

function initVertexBuffers()
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

function loadTexture(sampler, index, data, width, height)
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
    else if(index == 2)
    {
        gl.activeTexture(gl.TEXTURE2); //激活2号纹理
    }
    gl.bindTexture(gl.TEXTURE_2D, texture); //绑定纹理对象到激活的纹理单元
    // 配置纹理参数
    gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MAG_FILTER, gl.LINEAR);// 纹理放大方式
    gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MIN_FILTER, gl.LINEAR);// 纹理缩小方式
    gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_WRAP_S, gl.CLAMP_TO_EDGE);// 纹理水平填充方式
    gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_WRAP_T, gl.CLAMP_TO_EDGE);// 纹理垂直填充方式

    gl.texImage2D(gl.TEXTURE_2D, 0, gl.LUMINANCE, width, height, 0, gl.LUMINANCE, gl.UNSIGNED_BYTE, data);// 配置纹理图像
    gl.uniform1i(sampler, index); //将0号纹理传递给着色器
}

function initTextures(data, width, height)
{
    var u_SamplerY = gl.getUniformLocation(gl.shaderProgram, "u_SamplerY");
    var u_SamplerU = gl.getUniformLocation(gl.shaderProgram, "u_SamplerU");
    var u_SamplerV = gl.getUniformLocation(gl.shaderProgram, "u_SamplerV");

    var uOffset = width * height;
    var vOffset = (width >> 1) * (height >> 1);

    loadTexture(u_SamplerY, 0, data.subarray(0, uOffset), width, height);
    loadTexture(u_SamplerU, 1, data.subarray(uOffset, uOffset + vOffset), width / 2, height / 2);
    loadTexture(u_SamplerV, 2, data.subarray(uOffset + vOffset, uOffset + 2 * vOffset), width / 2, height / 2);

    gl.clearColor(0.0, 0.0, 0.0, 1.0); //指定清空canvas的颜色
    gl.clear(gl.COLOR_BUFFER_BIT); //清空canvas
    gl.drawArrays(gl.TRIANGLE_STRIP, 0, 4);
}

var gl = null;
function mainprocess() {
    var canvas = document.getElementById("container");
    gl = canvas.getContext("webgl") || canvas.getContext("experimental-webgl");

    // gl.pixelStorei(gl.UNPACK_ALIGNMENT, 1);

    initShader(); //初始化着色器
    initVertexBuffers();
    // initTextures();
}

document.getElementById('upload').onchange = function(evt){
    var file = document.getElementById('upload').files[0]; //选择的文件对象
    if (file) {
        var start = 0;
        var step = width * height * 3 / 2;

        var interval = setInterval(() => {
			var reader = new FileReader();
			var blob = file.slice(start, start + step);
			start += step;
            reader.readAsArrayBuffer(blob);
            reader.onload = function () {
                var data = new Uint8Array(this.result);
                initTextures(data, width, height);
            }
            if(start >= file.size)
            {
                clearInterval(interval);
            }
        }, 40);
    }
}

window.onload = mainprocess();