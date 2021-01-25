
/**
 * 使用WebGL画点
 * 绘制鼠标点击过的位置，根据不同的象限显示不同的颜色
 * 在这个例子中用uniform变量传递颜色数据
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
precision mediump float; //设置精度
uniform vec4 u_FragColor;
void main(){
    gl_FragColor = u_FragColor; // 内存变量，表示片元颜色RGBA
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
    var a_PointSize = gl.getAttribLocation(shaderProgram, "a_PointSize"); //获取shader中的a_PointSize变量
    gl.vertexAttrib1f(a_PointSize, 10.0); //给变量a_PointSize赋值
    var a_Position = gl.getAttribLocation(shaderProgram, "a_Position"); //获取shader中的a_Position变量
    var u_FragColor = gl.getUniformLocation(shaderProgram, "u_FragColor"); //获取shader中的u_FragColor变量

    var points = [];
    var colors = [];
    canvas.onmousedown = function(event) {
        var rect = event.target.getBoundingClientRect();
        var x = ((event.clientX - rect.left) - canvas.width * 0.5) / (canvas.width * 0.5);
        var y = (canvas.height * 0.5 - (event.clientY - rect.top)) / (canvas.height * 0.5);
        points.push([x, y]);

        if(x > 0 && y > 0)
        {
            colors.push([1.0, 0.0, 0.0, 1.0]);
        }
        else if(x < 0 && y > 0)
        {
            colors.push([0.0, 1.0, 0.0, 1.0]);
        }
        else if(x < 0 && y < 0)
        {
            colors.push([0.0, 0.0, 1.0, 1.0]);
        }
        else if(x > 0 && y < 0)
        {
            colors.push([0.0, 1.0, 1.0, 1.0]);
        }

        gl.clear(gl.COLOR_BUFFER_BIT);// 清空canvas
        for(var i = 0; i < points.length; i++)
        {
            var pos = points[i];
            var rgba = colors[i];
            gl.vertexAttrib4f(a_Position, pos[0], pos[1], 0.0, 1.0); //给变量a_Position赋值
            gl.uniform4f(u_FragColor, rgba[0], rgba[1], rgba[2], rgba[3]);
            gl.drawArrays(gl.POINTS, 0, 1);// 画点
        }
    }
}
