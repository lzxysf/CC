/**
 * 使用WebGL画点
 * 使用VBO一次性向shader传递多个变量
 * gl.vertexAttribxxx 和 gl.uniformxxx 一次只能传递一个对象，要一次性传递多个对象就要用到顶点缓冲区对象VBO。
 */

/**
 * VBO使用步骤:
 * 创建缓冲区对象:gl.createBuffer()
 * 绑定缓冲区对象:gl.bindBuffer()
 * 将数据写入缓冲区:gl.bufferData()
 * 将缓冲区对象分配给一个attribute变量:gl.vertexAttribPointer()
 * 开启attribute变量:gl.enableVertexAttribArray()
 */

/**
 * void gl.vertexAttribPointer(GLuint index, GLint size, GLenum type, 
 * GLboolean normalized, GLsizei stride,const GLvoid * pointer);
 * 指定了渲染时索引值为 index 的顶点属性数组的数据格式和位置
 * 
 * 参数:
 * index:指定要修改的顶点属性的索引值
 * size:指定每个顶点属性的组件数量。必须为1、2、3或者4。初始值为4。（如position是由3个（x,y,z）组成，而颜色是4个（r,g,b,a））
 * type:指定数组中每个组件的数据类型。可用的符号常量有GL_BYTE, GL_UNSIGNED_BYTE, GL_SHORT,GL_UNSIGNED_SHORT, GL_FIXED, 和 GL_FLOAT，初始值为GL_FLOAT。
 * normalized:指定当被访问时，固定点数据值是否应该被归一化（GL_TRUE）或者直接转换为固定点值（GL_FALSE）。
 * stride:指定连续顶点属性之间的偏移量。如果为0，那么顶点属性会被理解为：它们是紧密排列在一起的。初始值为0。
 * pointer:指定第一个组件在数组的第一个顶点属性中的偏移量。该数组与GL_ARRAY_BUFFER绑定，储存于缓冲区中。初始值为0；
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

    var a_PointSize = gl.getAttribLocation(shaderProgram, "a_PointSize"); //获取shader中的a_PointSize变量
    gl.vertexAttrib1f(a_PointSize, 10.0); //给变量a_PointSize赋值

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

    //最后一个参数3表示一次性向缓冲区写入3个顶点
    //改为1或2时，向缓冲区写入1个或2个顶点
    gl.drawArrays(gl.POINTS, 0, 3);

    /**
     * drawArrays第一个参数说明：
        POINTS：绘制一个个点单；
        LINES：在V0-V1，V2-V3，…之间划线，奇数个点最后一个会被忽略；
        LINE_STRIP: 依次收尾链接v0-v1-v2-v3…；
        LINE_LOOP：在所有点之间两两连线；
        TRIANGLES: 绘制三角形v0-v1-v2， v3-v4-v5，…；
        TRIANGLE_STRIP：任意连续三个点之间绘制三角形；
        TRIANGLE_FAN：复用上一个三角形的一边与下一个点，绘制一个三角形。
     */
    //gl.POINTS修改为gl.TRANGLES，就是以这三个点为顶点绘制三角形
    // gl.drawArrays(gl.TRIANGLES, 0, 3);
}
