
#ifndef __WEBPAGES_H
#define __WEBPAGES_H

#define WEB_PAGE "<!DOCTYPE html><html><head>"\
 	"<title>Gamma spectrometer</title>"\
	"<style>"\
        "body {"\
            "font-family: Arial, sans-serif;"\
            "margin: 0;"\
            "padding: 20px;"\
            "background-color: #f5f5f5;"\
        "}"\
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
        ".container {"\
            "display: flex;"\
            "max-width: 1300px;"\
            "margin: 0 auto;"\
            "background: white;"\
            "border-radius: 8px;"\
            "box-shadow: 0 2px 10px rgba(0,0,0,0.1);"\
            "overflow: hidden;"\
        "}"\
        ".control-panel {"\
            "width: 220px;"\
            "background: #2c3e50;"\
            "padding: 10px;"\
            "display: flex;"\
            "flex-direction: column;"\
            "color: white;"\
        "}"\
        ".control-panel h2 {"\
            "margin-top: 0;"\
            "color: #ecf0f1;"\
            "border-bottom: 1px solid #34495e;"\
            "padding-bottom: 10px;"\
        "}"\
        ".control-buttons {"\
            "display: flex;"\
            "flex-direction: column;"\
            "gap: 5px;"\
        "}"\
        ".add-btn, .submit-btn {"\
            "width: 100%;"\
            "padding: 5px;"\
            "border: none;"\
            "border-radius: 4px;"\
            "cursor: pointer;"\
            "font-size: 16px;"\
            "font-weight: bold;"\
            "transition: all 0.3s;"\
        "}"\
        ".add-btn {"\
            "background-color: #27ae60;"\
            "color: white;"\
            "margin-top: 5px;"\
        "}"\
        ".add-btn:hover {"\
            "background-color: #2ecc71;"\
        "}"\
        ".submit-btn {"\
            "background-color: #2980b9;"\
            "color: white;"\
            "margin-top: auto;"\
        "}"\
        ".submit-btn:hover {"\
            "background-color: #3498db;"\
        "}"\
        ".counter {"\
            "text-align: center;"\
            "padding: 5px;"\
            "background: #34495e;"\
            "border-radius: 4px;"\
            "margin-top: 5px;"\
        "}"\
        ".input-panel {"\
            "flex: 1;"\
            "padding: 5px;"\
        "}"\
        ".input-header {"\
            "display: flex;"\
            "background: #f1f1f1;"\
            "padding: 5px 5px;"\
            "border-radius: 4px;"\
            "margin-bottom: 5px;"\
            "font-weight: bold;"\
        "}"\
        ".input-header div {"\
            "flex: 1;"\
            "text-align: center;"\
        "}"\
        ".input-header .action-header {"\
            "width: 200px;"\
            "flex: none;"\
        "}"\
        ".input-row {"\
            "display: flex;"\
            "align-items: center;"\
            "padding: 5px 5px;"\
            "margin-bottom: 5px;"\
            "background: #f9f9f9;"\
            "border-radius: 4px;"\
            "border: 1px solid #eee;"\
            "transition: all 0.3s;"\
        "}"\
        ".input-row:hover {"\
            "background: #f1f8ff;"\
            "border-color: #d1e7ff;"\
        "}"\
        ".input-row input {"\
            "flex: 1;"\
            "padding: 1px;"\
            "margin: 0 5px;"\
            "border: 1px solid #ddd;"\
            "border-radius: 4px;"\
            "font-size: 16px;"\
        "}"\
        ".input-row .row-number {"\
            "width: 30px;"\
            "text-align: center;"\
            "font-weight: bold;"\
            "color: #7f8c8d;"\
        "}"\
        ".delete-btn {"\
            "width: 90px;"\
            "padding: 5px;"\
            "background-color: #e74c3c;"\
            "color: white;"\
            "border: none;"\
            "border-radius: 4px;"\
            "cursor: pointer;"\
            "transition: background-color 0.3s;"\
            "margin-left: 5px;"\
        "}"\
        ".delete-btn:hover {"\
            "background-color: #c0392b;"\
        "}"\
        ".select-btn {"\
            "width: 90px;"\
            "padding: 5px;"\
            "background-color: #27ae60;"\
            "color: white;"\
            "border: none;"\
            "border-radius: 4px;"\
            "cursor: pointer;"\
            "transition: background-color 0.3s;"\
        "}"\
        ".select-btn:hover {"\
           "background-color: #2ecc71;"\
        "}"\
        ".status-message {"\
            "margin-top: 20px;"\
            "padding: 15px;"\
            "border-radius: 4px;"\
            "display: none;"\
        "}"\
        ".success {"\
            "background-color: #d4edda;"\
            "color: #155724;"\
            "display: block;"\
        "}"\
        ".error {"\
            "background-color: #f8d7da;"\
            "color: #721c24;"\
            "display: block;"\
        "}"\
	"</style>"\
 	"</head><body>"\
		"<div class=\"container\">"\
			"<div class=\"specter\">"\
				"<canvas id=\"Chart\" width=\"1030\" height=\"700\"></canvas>"\
			"</div>"\
			"<div class=\"control-panel\">"\
				"<div>"\
					"<p class=\"counter\"> Count : <span id=\"counter_id\">0</span></p>"\
					"<p class=\"counter\">Time : <span id=\"time_id\">0</span></p>"\
					"<p class=\"counter\">CPS : <span id=\"cps_id\">0</p>"\
					"<p class=\"counter\">Aquracy : <span id=\"aquracy_id\">10</span>%</p>"\
					"<p class=\"counter\">CountSel : <span id=\"cnt_sel_id\">10</span></p>"\
					"<p class=\"counter\">CPSSel : <span id=\"cps_sel_id\">10</span></p>"\
					"<p class=\"counter\">AquracySel : <span id=\"aquracy_sel_id\">10</span>%</p>"\
				"</div>"\
				"<button class=\"add-btn\" type=\"button\" onclick=snd(\"/clr\")>Clear</button>"\
				"<button class=\"add-btn\" type=\"button\" onclick=saveSpr()>Save</button>"\
				"<button class=\"add-btn\" type=\"button\" onclick=snd(\"/rst\")>Reset</button>"\
			"</div>"\
		"</div>"\
		"<div class=\"container\">"\
			"<div class=\"control-panel\">"\
				"<div class=\"control-buttons\">"\
					"<button id=\"addRowBtn\" class=\"add-btn\">Add interval</button>"\
					"<button id=\"readSelBtn\" class=\"submit-btn\">Read intervals</button>"\
					"<button id=\"submitBtn\" class=\"submit-btn\">Submit intervals</button>"\
				"</div>"\
				"<div class=\"counter\">"\
					"Intervals: <span id=\"rowCounter\">0</span>/" TOSTRING(NUMBERINTERVAL)\
				"</div>"\
			"</div>"\
			"<div class=\"input-panel\">"\
				"<div class=\"input-header\">"\
					"<div class=\"row-number\">#</div>"\
					"<div>Channel</div>"\
					"<div>Width</div>"\
					"<div>Level</div>"\
					"<div class=\"action-header\">Action</div>"\
				"</div>"\
				"<div id=\"inputContainer\"></div>"\
				"<div id=\"statusMessage\" class=\"status-message\"></div>"\
			"</div>"\
		"</div>"\
		"<script>"\
		 	 "const canvas = document.getElementById('Chart');"\
		 	 "const ctx = canvas.getContext('2d');"\
			 "const cntr = document.getElementById('counter_id');"\
			 "const tmm = document.getElementById('time_id');"\
			 "const CPS = document.getElementById('cps_id');"\
			 "const aqur = document.getElementById('aquracy_id');"\
			 "const cntSel = document.getElementById('cnt_sel_id');"\
			 "const cpsSel = document.getElementById('cps_sel_id');"\
			 "const aqurSel = document.getElementById('aquracy_sel_id');"\
		 	 "var sptrArr = \"\";"\
			 "var doNotRedraw = true;"\
			 "let isDrawing = false;"\
			 "let startSelFlag = false;"\
			 "let startX, endX;"\
			 "const inputContainer = document.getElementById('inputContainer');"\
			 "const addRowBtn = document.getElementById('addRowBtn');"\
			 "const submitBtn = document.getElementById('submitBtn');"\
			 "let rowCount = 0;"\
			 "const maxRows = " TOSTRING(NUMBERINTERVAL) ";"\
			 "let sel_input_chan;"\
			 "let sel_input_with;"\
			 "let sel_input_btn;"\
			 "const data = [];"\
			 "function submitData() {"\
				"const rows = inputContainer.querySelectorAll('.input-row');"\
				"if (rows.length === 0) {"\
					"return;"\
				"}"\
				"let hasErrors = false;"\
				"data.length = 0;"\
				"rows.forEach((row, index) => {"\
					"const inputs = row.querySelectorAll('input');"\
					"const int1 = inputs[0].value;"\
					"const int2 = inputs[1].value;"\
					"const floatValue = inputs[2].value;"\
					"data.push({"\
						"int1: parseInt(int1),"\
						"int2: parseInt(int2),"\
						"floatValue: parseFloat(floatValue)"\
					"});"\
				"});"\
			 "}"\
			 "function addInputRow() {"\
				"if (rowCount >= maxRows) {"\
					"alert(`Maximum intervals: ${maxRows}`);"\
					"return;"\
				"}"\
				"const row = document.createElement('div');"\
				"row.className = 'input-row';"\
				"row.innerHTML = `"\
                "<div class=\"row-number\">${rowCount + 1}</div>"\
					"<input type=\"number\" placeholder=\"0\" class=\"int-input int_input1\" required>"\
					"<input type=\"number\" placeholder=\"0\" class=\"int-input int_input2\" required>"\
					"<input type=\"number\" placeholder=\"0\" step=\"any\" class=\"float-input\" required>"\
					"<button class=\"select-btn action-btn\">Sel</button>"\
					"<button class=\"delete-btn\">Del</button>`;"\
				"inputContainer.appendChild(row);"\
				"rowCount++;"\
				"rowCounter.textContent = rowCount;"\
				"row.querySelector('.action-btn').addEventListener('click', () => {"\
					"var txt = row.querySelector('.action-btn').innerText;"\
					"if (txt == \"Sel\") {"\
						"row.querySelector('.action-btn').innerText = \"Clr\";"\
						"sel_input_chan = row.querySelector('.int_input1');"\
						"sel_input_with = row.querySelector('.int_input2');"\
						"sel_input_btn = row.querySelector('.action-btn');"\
						"row.querySelector('.action-btn').classList.replace(\"select-btn\", \"delete-btn\");"\
						"startSelFlag = true;"\
					"} else {"\
						"row.querySelector('.action-btn').innerText = \"Sel\";"\
						"row.querySelector('.action-btn').classList.replace(\"delete-btn\", \"select-btn\");"\
						"startSelFlag = false;"\
					"}"\
				"});"\
				"row.querySelector('.delete-btn').addEventListener('click', () => {"\
					"row.remove();"\
					"rowCount--;"\
					"if (rowCount == 0) {"\
						"data.length = 0;"\
					"}"\
					"rowCounter.textContent = rowCount;"\
					"updateRowNumbers();"\
				"});"\
			 "}"\
			 "function updateRowNumbers() {"\
				"const rows = inputContainer.querySelectorAll('.input-row');"\
				"rows.forEach((row, index) => {"\
					"row.querySelector('.row-counter').textContent = index + 1;"\
				"});"\
			 "}"\
			 "function getMousePosition(e) {"\
				"const rect = canvas.getBoundingClientRect();"\
				"return {x: e.clientX - rect.left};"\
			 "}"\
			 ""\
			 "canvas.addEventListener('mousedown', (e) => {"\
				"if (!startSelFlag) return;"\
				"if (e.button !== 0) return;"\
				"doNotRedraw = false;"\
				"const pos = getMousePosition(e);"\
				"isDrawing = true;"\
				"startX = endX = pos.x;"\
				"backgroundImageData = ctx.getImageData(0, 0, canvas.width, canvas.height);"\
			 "});"\
			 ""\
			 "canvas.addEventListener('mousemove', (e) => {"\
				"if (!isDrawing) return;"\
				"const pos = getMousePosition(e);"\
				"endX = pos.x;"\
				"ctx.putImageData(backgroundImageData, 0, 0);"\
				"const x = Math.min(startX, endX);"\
				"const width = Math.abs(endX - startX);"\
				"ctx.strokeStyle = '#ff0000';"\
				"ctx.lineWidth = 2;"\
				"ctx.strokeRect(x, 0, width, canvas.height);"\
				"ctx.fillStyle = 'rgba(255, 0, 0, 0.1)';"\
				"ctx.fillRect(x, 0, width, canvas.height);"\
			 "});"\
			 ""\
			 "canvas.addEventListener('mouseup', (e) => {"\
				"if (!isDrawing) return;"\
				"isDrawing = false;"\
				"doNotRedraw = true;"\
				"const pos = getMousePosition(e);"\
				"endX = pos.x;"\
				"const x1 = Math.min(startX, endX);"\
				"const x2 = Math.max(startX, endX);"\
				"sel_input_chan.value = x1 + Math.round((x2 - x1) / 2);"\
				"sel_input_with.value = x2 - x1;"\
				"sel_input_btn.innerText = \"Sel\";"\
				"sel_input_btn.classList.replace(\"delete-btn\", \"select-btn\");"\
				"startSelFlag = false;"\
			 "});"\
			 ""\
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
			 "function reloadParameters(countr, tmr, cntrsel) {"\
		 		 "cntr.innerText = countr;"\
		 		 "tmm.innerText = tmr;"\
		 		 "CPS.innerText = (countr / tmr).toFixed(2);"\
		 		 "aqur.innerText = Math.round(300 / Math.sqrt(countr));"\
		 		 "cntSel.innerText = cntrsel;"\
		 		 "cpsSel.innerText = (cntrsel / tmr).toFixed(4);"\
		 		 "aqurSel.innerText = Math.round(300 / Math.sqrt(cntrsel));"\
		 	 "}"\
		 	 "function drawSpecter(dataArray) {"\
		 		 "if (!doNotRedraw) return;"\
		 		 "sptrArr = \"\";"\
		 		 "ctx.clearRect(0, 0, canvas.width, canvas.height);"\
		 	 	 "old_x = 0;"\
		 	 	 "old_y = 0;"\
		 	 	 "old_y_lg = 0;"\
		 	 	 "cur_x = 0;"\
		 	 	 "cur_log_y = 0;"\
		 	 	 "const max_y = Math.max(...dataArray);"\
		 	 	 "const koeff = canvas.height / max_y;"\
		 	 	 "const koefflg = canvas.height / Math.log(max_y);"\
		 	 	 "dataArray.forEach((value) => {"\
		 	 		 "sptrArr += value + \"\\n\";"\
		 	 		 "drawLine(old_x, canvas.height - old_y * koeff, cur_x, canvas.height - value * koeff, 'green', 1);"\
		 	 		 "if(value != 0){"\
					 	 "cur_log_y = Math.log(value);"\
					 "} else {"\
						 "cur_log_y = 0;"\
					 "}"\
		 	 		 "drawLine(old_x, canvas.height - old_y_lg * koefflg, cur_x, canvas.height - cur_log_y * koefflg, 'yellow', 1);"\
		 	 		 "cur_x++;"\
		 	 		 "old_x = cur_x;"\
		 	 		 "old_y = value;"\
		 	 		 "old_y_lg = cur_log_y;"\
			 "});"\
				"submitData();"\
				"if (data.length > 0) {"\
					"data.forEach((dat) => {"\
						"var x_sel = Number(dat.int1) - Number(dat.int2 / 2);"\
						"if ((x_sel > 2) && (dat.int2 > 10)) {"\
							"ctx.strokeStyle = '#007f00';"\
							"ctx.lineWidth = 2;"\
							"ctx.strokeRect(x_sel, 0, dat.int2, canvas.height);"\
							"ctx.fillStyle = 'rgba(0, 255, 0, 0.1)';"\
							"ctx.fillRect(x_sel, 0, dat.int2, canvas.height);"\
						"}"\
					"});"\
				"}"\
			 "}"\
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
						"reloadParameters(data.Counter, data.Time, data.countSel);"\
				  "}).catch(error => {"\
						"console.error('Error:', error);"\
			 "})};"\
			 "function sendSelectData() {"\
				"submitData();"\
				"if (data.length > 0) {"\
					"var selData = \"\";"\
					"var a = 0;"\
					"var b = 0;"\
					"var c = 0;"\
					"var startInt = 0;"\
					"var endInt = 0;"\
					"data.forEach((dat) => {"\
						"if (isNaN(dat.int1)) {"\
							"a = 0;"\
						"} else {"\
							"a = dat.int1;"\
						"}"\
						"if (isNaN(dat.int2)) {"\
							"b = 0;"\
						"} else {"\
							"b = dat.int2;"\
						"}"\
						"if (isNaN(dat.floatValue)) {"\
							"c = 0;"\
						"} else {"\
							"c = dat.floatValue;"\
						"}"\
						"startInt = Math.round(Number(a) - Number(b / 2));"\
						"endInt = Math.round(Number(a) + Number(b / 2));"\
						"selData += startInt + \"|\" + endInt + \"|\" + c + \"|\";"\
					"});"\
					"console.log(data);"\
					"snd(\"/sel\" + selData);"\
				"}else{"\
					"console.log(\"No data\");"\
				"}"\
			 "}"\
			 "addRowBtn.addEventListener('click', addInputRow);"\
			 "submitBtn.addEventListener('click', sendSelectData);"\
			 "window.addEventListener('DOMContentLoaded', addInputRow);"\
  	  	  	 "setInterval(fetchData, 1000);"\
			 "fetchData();"\
			 "</script>"\
	"</body></html>"

#define SPECTER_DATA_1 "HTTP/1.1 200 OK\r\n"\
						"Content-Type: application/json\r\n\r\n{"

#define SPECTER_DATA_2 "\"Counter\": "
#define SPECTER_DATA_3 "\"Time\": "
#define SPECTER_DATA_4 "\"countSel\": "
#define SPECTER_DATA_5 "\"Aquracy\": "

#define SPECTER_DATA_6 "\"dataArray\": ["
#define SPECTER_DATA_7 "],"
#define SPECTER_DATA_8 "}"
#define SPECTER_DATA_9 "HTTP/1.1 200 OK\n\n"

#define LENGTH_WEB_PAGE (sizeof(WEB_PAGE) - 1)
#define LENGTH_SPECTER_DATA_1 (sizeof(SPECTER_DATA_1) - 1)
#define LENGTH_SPECTER_DATA_2 (sizeof(SPECTER_DATA_2) - 1)
#define LENGTH_SPECTER_DATA_3 (sizeof(SPECTER_DATA_3) - 1)
#define LENGTH_SPECTER_DATA_4 (sizeof(SPECTER_DATA_4) - 1)
#define LENGTH_SPECTER_DATA_5 (sizeof(SPECTER_DATA_5) - 1)
#define LENGTH_SPECTER_DATA_6 (sizeof(SPECTER_DATA_6) - 1)
#define LENGTH_SPECTER_DATA_7 (sizeof(SPECTER_DATA_7) - 1)
#define LENGTH_SPECTER_DATA_8 (sizeof(SPECTER_DATA_8) - 1)
#define LENGTH_SPECTER_DATA_9 (sizeof(SPECTER_DATA_9) - 1)

#endif
