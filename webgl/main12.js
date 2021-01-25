/**
 * WebGL——矩阵变换——组合变换
 * 用矩阵实现图形的平移、旋转、缩放的组合变换
 * 本例中，先平移(0.3, 0.3)，后旋转90°，再缩放为1/2
 */

/**
 * 比如，对图像g进行一次平移，然后进行一次缩放，求最终的得到图像g
 * 平移后的坐标t = 平移矩阵 x 原始坐标g
 * 缩放后的G = 缩放矩阵 x 平移后的坐标t
 * 缩放后的G = 缩放矩阵 x (平移矩阵 x 原始坐标g) = (缩放矩阵 x 平移矩阵) x 原始坐标g
 * 可见，对于多次变换，可以将变换矩阵相乘得到一个最终的矩阵（叫模型矩阵），然后传给顶点着色器做最终的运算。
 */

// 顶点着色器源码
var vertexShaderSrc = `
attribute vec4 a_Position; //该变量接收传入的位置坐标数值，必须声明为全局
uniform mat4 u_Mat1; //矩阵，可用于平移旋转缩放等
uniform mat4 u_Mat2; //矩阵，可用于平移旋转缩放等
uniform mat4 u_Mat3; //矩阵，可用于平移旋转缩放等
void main(){
    gl_Position = u_Mat1 * u_Mat2 * u_Mat3 * a_Position;
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

    //平移矩阵
    var mat3 = getTranslationMatrix(0.2, 0.2, 0.0);
    //旋转矩阵
    var mat2 = getRotationMatrix(Math.PI/2, 0, 0, 1);
    //缩放矩阵
    var mat1 = getScaleMatrix(0.5, 0.5, 0.5);

    var u_Mat1 = gl.getUniformLocation(shaderProgram, "u_Mat1");
    gl.uniformMatrix4fv(u_Mat1, false, mat1);
    var u_Mat2 = gl.getUniformLocation(shaderProgram, "u_Mat2");
    gl.uniformMatrix4fv(u_Mat2, false, mat2);
    var u_Mat3 = gl.getUniformLocation(shaderProgram, "u_Mat3");
    gl.uniformMatrix4fv(u_Mat3, false, mat3);

    gl.drawArrays(gl.TRIANGLES, 0, 3);
}

/**
 * 由平移向量获取平移矩阵
 */
function getTranslationMatrix(x, y, z) {
    return new Float32Array([
        1.0, 0.0, 0.0, 0.0,
        0.0, 1.0, 0.0, 0.0,
        0.0, 0.0, 1.0, 0.0,
        x, y, z, 1.0,
    ]);
}

/**
 * 由旋转弧度和旋转轴获取旋转矩阵
 */
function getRotationMatrix(rad, x, y, z) {
    if (x > 0) {
        // 绕x轴的旋转矩阵
        return new Float32Array([
            1.0, 0.0, 0.0, 0.0,
            0.0, Math.cos(rad), -Math.sin(rad), 0.0,
            0.0, Math.sin(rad), Math.cos(rad), 0.0,
            0.0, 0.0, 0.0, 1.0,
        ]);
    } else if (y > 0) {
        // 绕y轴的旋转矩阵
        return new Float32Array([
            Math.cos(rad), 0.0, -Math.sin(rad), 0.0,
            0.0, 1.0, 0.0, 0.0,
            Math.sin(rad), 0.0, Math.cos(rad), 0.0,
            0.0, 0.0, 0.0, 1.0,
        ]);
    } else if(z > 0) {
        // 绕z轴的旋转矩阵
        return new Float32Array([
            Math.cos(rad), Math.sin(rad), 0.0, 0.0,
            -Math.sin(rad), Math.cos(rad), 0.0, 0.0,
            0.0, 0.0, 1.0, 0.0,
            0.0, 0.0, 0.0, 1.0,
        ]);
    } else {
        // 没有指定旋转轴，报个错，返回一个单位矩阵
        console.error("error: no axis");
        return new Float32Array([
            1.0, 0.0, 0.0, 0.0,
            0.0, 1.0, 0.0, 0.0,
            0.0, 0.0, 1.0, 0.0,
            0.0, 0.0, 0.0, 1.0,
        ]);
    }
}

/**
 * 由缩放因子获取缩放矩阵
 */
function getScaleMatrix(xScale, yScale, zScale) {
    return new Float32Array([
        xScale, 0.0, 0.0, 0.0,
        0.0, yScale, 0.0, 0.0,
        0.0, 0.0, zScale, 0.0,
        0.0, 0.0, 0.0, 1.0,
    ]);
}
