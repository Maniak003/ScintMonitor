
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
        ".input-header {"\
            "display: grid;"\
            "grid-template-columns: 40px 100px 100px 100px 100px 180px 70px 70px;"\
            "background: #2c3e50;"\
            "color: white;"\
            "padding: 5px 5px;"\
            "border-radius: 4px;"\
            "margin-bottom: 10px;"\
            "font-weight: bold;"\
            "position: sticky;"\
            "top: 0;"\
            "z-index: 10;"\
            "gap: 10px;"\
        "}"\
        ".input-row {"\
            "display: grid;"\
            "grid-template-columns: 40px 100px 100px 100px 100px 180px 70px 70px;"\
            "align-items: center;"\
            "padding: 5px 5px;"\
            "margin-bottom: 10px;"\
            "background: white;"\
            "border-radius: 4px;"\
            "border: 1px solid #e0e0e0;"\
            "box-shadow: 0 1px 3px rgba(0,0,0,0.05);"\
            "gap: 10px;"\
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
            "padding: 5px 10px;"\
            "background-color: #e74c3c;"\
            "color: white;"\
            "border: none;"\
            "border-radius: 4px;"\
            "cursor: pointer;"\
            "transition: background-color 0.3s;"\
            "font-size: 12px;"\
        "}"\
        ".delete-btn:hover {"\
            "background-color: #c0392b;"\
        "}"\
        ".select-btn {"\
            "padding: 5px 10px;"\
            "background-color: #27ae60;"\
            "color: white;"\
            "border: none;"\
            "border-radius: 4px;"\
            "cursor: pointer;"\
            "transition: background-color 0.3s;"\
            "font-size: 12px;"\
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
		".cps-inp {"\
			"margin-left: 20px;"\
			"margin-right: 20px;"\
		"}"\
		".int-input .float-input {"\
			"width: 10px;"\
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
					"<div>#</div>"\
					"<div>Channel</div>"\
					"<div>Width</div>"\
					"<div>Level</div>"\
					"<div>Accuracy</div>"\
					"<div>CPS</div>"\
					"<div>Select</div>"\
					"<div>Delete</div>"\
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
			 "const readSelBtn = document.getElementById('readSelBtn');"\
			 "let rowCount = 0;"\
			 "const maxRows = " TOSTRING(NUMBERINTERVAL) ";"\
			 "let sel_input_chan;"\
			 "let sel_input_with;"\
			 "let sel_input_btn;"\
			 "let totalTime = 0;"\
			 "var need_reload_interval = true;"\
			 "const data = [];"\
			 "function FloatTo32BitInt(num) {"\
				"const buffer = new ArrayBuffer(4);"\
				"const view = new DataView(buffer);"\
				"view.setFloat32(0, num, false);"\
				"return view.getUint32(0, false);"\
			 "}"\
			 "function uint32ToFloat(intValue) {"\
				"const buffer = new ArrayBuffer(4);"\
				"const view = new DataView(buffer);"\
				"view.setUint32(0, intValue, false);"\
				"return view.getFloat32(0, false).toFixed(4);"\
			 "}"\
			 "function updateCpsValue(rowIndex, value, aqurs) {"\
    			"const cpsElements = document.querySelectorAll('.cps-inp');"\
    			"if (rowIndex < cpsElements.length) {"\
    				"cpsElements[rowIndex].textContent = (value / totalTime).toFixed(4) + \" (\" + (aqurs/Math.sqrt(value)).toFixed(2) + \"%)\";"\
    			"}"\
    		"}"\
			 "function reloadIntervals(Intervals) {"\
			 	"if (need_reload_interval) {"\
			 		"Intervals.forEach((interval, index) => {"\
				 		 "console.log(\"Add intervals.\");"\
				 		 "if (interval[0] != 0) {"\
							"var wh = interval[1] - interval[0];"\
							"var ch = Number(interval[0]) + Math.round(wh / 2);"\
							"addInputRow(ch, wh, uint32ToFloat(interval[2]));"\
						"}"\
					"});"\
					"need_reload_interval = false;"\
				"}"\
				"const cpsElements = document.querySelectorAll('.cps-inp');"\
            	"Intervals.forEach((interval, index) => {"\
            		"if (index < cpsElements.length) {"\
            			"updateCpsValue(index, interval[4], Math.sqrt(interval[3]) * 10);"\
            		"}"\
            	"});"\
 				"let hasErrors = false;"\
				"data.length = 0;"\
			 "}"\
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
					"const aqur = inputs[3].value;"\
					"data.push({"\
						"int1: parseInt(int1),"\
						"int2: parseInt(int2),"\
						"floatValue: parseFloat(floatValue),"\
						"aqur: parseInt(aqur)"\
					"});"\
				"});"\
			 "}"\
			 "function addInputRow(selChan = 0, selWidth = 0, selLev = 0) {"\
				"if (rowCount >= maxRows) {"\
					"alert(`Maximum intervals: ${maxRows}`);"\
					"return;"\
				"}"\
				"const row = document.createElement('div');"\
				"row.className = 'input-row';"\
				"row.innerHTML = `"\
                "<div class=\"row-number\">${rowCount + 1}</div>"\
					"<input type=\"number\" placeholder=\"0\" class=\"int-input int_input1\" min=5 max=1018 required>"\
					"<input type=\"number\" placeholder=\"0\" class=\"int-input int_input2\" min=10 max=1023 required>"\
					"<input type=\"number\" placeholder=\"0\" step=\"any\" class=\"float-input\" max=1000 min=0 required>"\
					"<input type=\"number\" placeholder=\"0\" class=\"int-input int_input3\" value=900 max=90000 min=100 required>"\
					"<span class=\"cps-inp\">0.000000</span>"\
					"<button class=\"select-btn action-btn\">Sel</button>"\
					"<button class=\"delete-btn\">Del</button>`;"\
				"inputContainer.appendChild(row);"\
				"rowCount++;"\
				"rowCounter.textContent = rowCount;"\
				"if (selChan != 0 && selWidth != 0) {"\
					"row.querySelector('.int_input1').value = selChan;"\
					"row.querySelector('.int_input2').value = selWidth;"\
					"row.querySelector('.float-input').value = selLev;"\
				"}"\
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
		 		 "aqur.innerText = (300 / Math.sqrt(countr)).toFixed(2);"\
		 		 "cntSel.innerText = cntrsel;"\
		 		 "cpsSel.innerText = (cntrsel / tmr).toFixed(4);"\
		 		 "aqurSel.innerText = (300 / Math.sqrt(cntrsel)).toFixed(2);"\
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
						"totalTime = data.Time;"\
						"reloadParameters(data.Counter, data.Time, data.countSel);"\
						"reloadIntervals(data.Intervals);"\
						"data=null;"\
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
					"var d = 0;"\
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
							"c = FloatTo32BitInt(dat.floatValue);"\
						"}"\
						"if (isNaN(dat.aqur)) {"\
							"d = 900;"\
						"} else {"\
							"d = dat.aqur"\
						"}"\
						"startInt = Math.round(Number(a) - Number(b / 2));"\
						"endInt = Math.round(Number(a) + Number(b / 2));"\
						"selData += startInt + \"|\" + endInt + \"|\" + c + \"|\" + d + \"|\";"\
					"});"\
					"console.log(data);"\
					"snd(\"/sel\" + selData);"\
				"}else{"\
					"console.log(\"No data\");"\
				"}"\
			 "}"\
			 "function setReloadData() {"\
				 "need_reload_interval = true;"\
			 "}"\
			 "addRowBtn.addEventListener('click', addInputRow);"\
			 "submitBtn.addEventListener('click', sendSelectData);"\
			 "readSelBtn.addEventListener('click', setReloadData);"\
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
#define SPECTER_DATA_A ",\"Intervals\": ["
#define SPECTER_DATA_B "]"

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
#define LENGTH_SPECTER_DATA_A (sizeof(SPECTER_DATA_A) - 1)
#define LENGTH_SPECTER_DATA_B (sizeof(SPECTER_DATA_B) - 1)


#endif
