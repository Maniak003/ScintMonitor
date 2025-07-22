
#ifndef __WEBPAGES_H
#define __WEBPAGES_H

#define WEB_PAGE1 "<!DOCTYPE html><html><head>"\
 	"<title>Gamma spectrometer</title>"\
	"<style>"\
		"canvas {"\
			"border: 0px solid #000;"\
			"background-color: #303030;"\
		"}"\
		".specter {"\
			"border: 1px solid #ccc;"\
			"border-radius: 10px;"\
			"padding: 10px;"\
			"text-align: center;"\
			"display: inline-block;"\
		"}"\
		".controlsb {"\
			"border: 1px solid #ccc;"\
			"border-radius: 10px;"\
			"padding: 10px;"\
			"text-align: left;"\
			"display: inline-block;"\
		"}"\
		".grid-container {"\
			"float: center;"\
			"display: flex;"\
		"}"\
		".button_stl {"\
			"padding: 0.5em 1.5em;"\
			"background-image: linear-gradient(rgba(158,158,158,0.5), rgb(158,158,158), rgba(158,158,158,0.5));"\
			"color: #fff;"\
			"font-size: 0.9em;"\
			"border: 2px solid rgb(158,158,158);"\
			"border-radius: 0.4em;"\
			"margin-bottom: 1rem;"\
			"margin-left: 0.5rem;"\
			"display: block;"\
		"}"\
		".label_stl {"\
			"padding: 10px;"\
			"display: flex;"\
		"}"\
		".input_const {"\
			"margin-left: 0.5rem;"\
			"display: flex;"\
		"}"\
	"</style>"\
 	"</head><body>"\
		"<div class=\"grid-container\">"\
			"<div class=\"specter\">"\
				"<canvas id=\"Chart\" width=\"1030\" height=\"600\"></canvas>"\
			"</div>"\
			"<div class=\"controlsb\">"\
				"<div>"\
					"<p> Counter : <span id=\"counter_id\">00000 </span></p>"\
					"<p>Time : <span id=\"time_id\">000000</span></p>"\
					"<p>CPS : <span id=\"cps_id\">00000</p>"\
					"<p>Aquracy : <span id=\"aquracy_id\">10</span>%</p>"\
				"</div>"\
				"<button class=\"button_stl\" type=\"button\" onclick=snd(\"/clr\")>Clear</button>"\
				"<button class=\"button_stl\" type=\"button\" onclick=saveSpr()>Save</button>"\
				"<button class=\"button_stl\" type=\"button\" onclick=snd(\"/rst\")>Reset</button>"\
			"</div>"\
		"</div>"\
		"<div class=\"label_stl\">"\
			"<p class=\"input_const\"> Channel : <input type=\"number\" id=\"chan_id\"  min=\"1\" max=\"1024\" value=\"1\"></input></p>"\
			"<p class=\"input_const\"> Width : <input type=\"number\" id=\"width_id\"  min=\"1\" max=\"99\" value=\"1\"></input></span>  </p>"\
			"<p class=\"input_const\"> Weight : <input type=\"number\" id=\"weight_id\"  min=\"1\" max=\"99\" value=\"1\"></input></p>"\
			"<button class=\"button_stl\" type=\"button\">New</button>"\
			"<button class=\"button_stl\" type=\"button\">Delete</button>"\
			"<button class=\"button_stl\" type=\"button\">Aplly</button>"\
		"</div>"

#define WEB_PAGE2 "<script>"\
		 	 "const canvas = document.getElementById('Chart');"\
		 	 "const ctx = canvas.getContext('2d');"\
		 	 "var sptrArr = \"\";"\
			 "function snd(fnc) {"\
				"req = new XMLHttpRequest();"\
				"req.open(\"GET\", fnc, true);"\
				"req.send();"\
			 "}"\
			 "function saveSpr() {"\
				"const contDisp=\"sptr.csv\";"\
				"var a = document.createElement(\"a\");"\
				"var file = new Blob([sptrArr], { type: \"'text/plain'\"});"\
				"a.href = window.URL.createObjectURL(file);"\
				"a.download=contDisp.replace(/\\\"/g,'');"\
				"a.click();"\
			 "}"\
		 	 "function drawLine(x1, y1, x2, y2, color = '#000', width = 2) {"\
		 	 	 "ctx.beginPath();"\
		 	 	 "ctx.strokeStyle = color;"\
		 	 	 "ctx.lineWidth = width;ctx.moveTo(x1, y1);"\
		 	 	 "ctx.lineTo(x2, y2);ctx.stroke();"\
		 	 "}"\
			 "function reloadParameters(cntr, tmr) {"\
		 		 "counter_id.innerText = cntr;"\
		 		 "time_id.innerText = tmr;"\
		 		 "cps_id.innerText = (cntr / tmr).toFixed(2);"\
		 		 "aquracy_id.innerText = Math.round(900 / Math.sqrt(cntr));"\
		 	 "}"\
		 	 "function drawSpecter(dataArray) {"\
		 		 "sptrArr = \"\";"\
		 		 "ctx.clearRect(0, 0, canvas.width, canvas.height);"\
		 	 	 "old_x = 0;"\
		 	 	 "old_y = 0;"\
		 	 	 "old_y_lg = 0;"\
		 	 	 "cur_x = 0;"\
		 	 	 "cur_log_y = 0;"\
		 	 	 "const max_y = Math.max(...dataArray);"\
		 	 	 "const koeff = 600 / max_y;"\
		 	 	 "const koefflg = 600 / Math.log(max_y);"\
		 	 	 "dataArray.forEach((value) => {"\
		 	 		 "sptrArr += value + \"\\n\";"\
		 	 		 "drawLine(old_x, 600 - old_y * koeff, cur_x, 600 - value * koeff, 'green', 1);"\
		 	 		 "if(value != 0){"\
					 	 "cur_log_y = Math.log(value);"\
					 "} else {"\
						 "cur_log_y = 0;"\
					 "}"\
		 	 		 "drawLine(old_x, 600 - old_y_lg * koefflg, cur_x, 600 - cur_log_y * koefflg, 'yellow', 1);"\
		 	 		 "cur_x++;"\
		 	 		 "old_x = cur_x; "\
		 	 		 "old_y = value;"\
		 	 		 "old_y_lg = cur_log_y;"\
			 "});}"\
			 "function fetchData() {"\
				  "fetch('/data').then(response => {"\
					"const contentType = response.headers.get('content-type');"\
					"if (contentType.includes('application/json')) {"\
					  "return response.json();"\
					"}"\
					"return response.text().then(text => {"\
					  "try {"\
						"return JSON.parse(text);"\
					  "} catch (e) {"\
						"throw new Error(\"Invalid JSON: \" + text);"\
					  "}"\
					"});"\
				  "}).then(data => {"\
						"drawSpecter(data.dataArray);"\
						"reloadParameters(data.Counter, data.Time);"\
				  "}).catch(error => {"\
						"console.error('Error:', error);"\
			 "})};"\
  	  	  	 "setInterval(fetchData, 1000);"\
			 "fetchData();"\
			 "</script>"\
	"</body></html>"

#define SPECTER_DATA_1 "HTTP/1.1 200 OK\r\n"\
						"Content-Type: application/json\r\n\r\n{"

#define SPECTER_DATA_2 "\"Counter\": "
#define SPECTER_DATA_3 "\"Time\": "
#define SPECTER_DATA_4 "\"CPS\": "
#define SPECTER_DATA_5 "\"Aquracy\": "

#define SPECTER_DATA_6 "\"dataArray\": ["
#define SPECTER_DATA_7 "]}"

#define LENGTH_WEB_PAGE1 (sizeof(WEB_PAGE1) - 1)
#define LENGTH_WEB_PAGE2 (sizeof(WEB_PAGE2) - 1)
#define LENGTH_SPECTER_DATA_1 (sizeof(SPECTER_DATA_1) - 1)
#define LENGTH_SPECTER_DATA_2 (sizeof(SPECTER_DATA_2) - 1)
#define LENGTH_SPECTER_DATA_3 (sizeof(SPECTER_DATA_3) - 1)
#define LENGTH_SPECTER_DATA_4 (sizeof(SPECTER_DATA_4) - 1)
#define LENGTH_SPECTER_DATA_5 (sizeof(SPECTER_DATA_5) - 1)
#define LENGTH_SPECTER_DATA_6 (sizeof(SPECTER_DATA_6) - 1)
#define LENGTH_SPECTER_DATA_7 (sizeof(SPECTER_DATA_7) - 1)

#endif

/*
 * <!DOCTYPE html><html><head>
 * <title>Gamma spectrometer</title>
 * <style>canvas {border: 1px solid #000;background-color: #f0f0f0;}</style>
 * </head><body>
 * <canvas id="myCanvas" width="1200" height="600"></canvas>
 * <script>
 * const canvas = document.getElementById('myCanvas');
 * const ctx = canvas.getContext('2d');
 * function drawPoint(x, y, color = '#000', size = 5) {ctx.beginPath();
 * ctx.fillStyle = color;
 * ctx.arc(x, y, size, 0, Math.PI * 2);
 * ctx.fill();
 * }
 * function drawLine(x1, y1, x2, y2, color = '#000', width = 2) {
 * ctx.beginPath();
 * ctx.strokeStyle = color;
 * ctx.lineWidth = width;ctx.moveTo(x1, y1);
 * ctx.lineTo(x2, y2);ctx.stroke();
 * }
 * function drawSpecter() {
 * old_x = 0;
 * old_y = 0;
 * const points = [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,4,0,3,0,1,0,0,1,0,0,1,0,0,0,1,1,2,1,1,2,1,2,3,0,2,1,1,3,1];
 * const koeff = 600 / Math.max(...points);
 * for (let i = 0; i < points.length; i++) {
 * drawLine(old_x * 10, 600 - old_y * koeff, i * 10, 600 - points[i] * koeff, 'green', 1);
 * old_x = i;
 * old_y = points[i];
 * }
 * }
 * window.onload = drawSpecter;
 * </script></body></html>
 *
 *
 */
