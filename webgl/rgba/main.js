/**
    使用canvas绘制rgba视频，目的是与使用webgl绘制yuv视频相对比，对比CPU、GPU以及内存使用情况。
 */

//需要根据传入的rgba视频文件的分辨率来修改此处宽高值
var width = 1920;
var height = 1080;

var context = null;
var imgData = null;

function mainprocess() {
    var canvas = document.getElementById("container");
    //注意此处必须设置宽高，否则视频显示不全，canvas中只能显示视频的左上角
    //此处的width height类似于<canvas width="1920" height="1080"></canvas>中宽高设置，和CSS中宽高设置不同
    //html中canvas标签是相对布局，width height的变化不会影响实际canvas尺寸的变化。
	canvas.width = width;
	canvas.height = height;
    context = canvas.getContext('2d');
    imgData = context.createImageData(width, height); 
}

function renderImage(data)
{
   for(let i = 0; i < data.length; i++)
   {
       imgData.data[i] = data[i];
   }
   context.putImageData(imgData, 0, 0);
}

document.getElementById('upload').onchange = function(evt){
    var file = document.getElementById('upload').files[0]; //选择的文件对象
    if (file) {
        var start = 0;
        var step = width * height * 4;

        var interval = setInterval(() => {
		var reader = new FileReader();
		var blob = file.slice(start, start + step);
		start += step;
        	reader.readAsArrayBuffer(blob);
         	reader.onload = function () {
                    var data = new Uint8Array(this.result);
                    renderImage(data);
            	}	
                if(start >= file.size)
                {
                    //clearInterval(interval);
					start = 0; //从头开始渲染，循环渲染
                }
       }, 40);
    }
}

window.onload = mainprocess();
