


//#define w5x00web_chart "file:///home/ed/work/github/ScintMonitor/stm32cubeide/WEB_Interface/chart.js"

#define Spectrometr_page "<!DOCTYPE html><html><head>"\
    "<title>Gamma spectrometer</title>"\
    "<style>"\
        "canvas {"\
            "border: 1px solid #000;"\
            "background-color: #f0f0f0;}"\
    "</style>"\
"</head>"\
"<body>"\
    "<canvas id=\"myCanvas\" width=\"1200\" height=\"600\"></canvas>"\
    "<script>"\
        "const canvas = document.getElementById('myCanvas');"\
        "const ctx = canvas.getContext('2d');"\
        "function drawPoint(x, y, color = '#000', size = 5) {"\
            "ctx.beginPath();"\
            "ctx.fillStyle = color;"\
            "ctx.arc(x, y, size, 0, Math.PI * 2);"\
            "ctx.fill();}"\
        "function drawLine(x1, y1, x2, y2, color = '#000', width = 2) {"\
            "ctx.beginPath();"\
            "ctx.strokeStyle = color;"\
            "ctx.lineWidth = width;"\
            "ctx.moveTo(x1, y1);"\
            "ctx.lineTo(x2, y2);"\
            "ctx.stroke();"\
        "}"\
        "function drawSpecter() {"\
        	"old_x = 0; old_y = 0;"\
            "const points = [%s];"\
            "const koeff = 600 / Math.max(...points);"\
            "for (let i = 0; i < points.length; i++) {"\
                "drawLine(old_x * 10, 600 - old_y * koeff, i * 10, 600 - points[i] * koeff, 'green', 1);"\
                "old_x = i; old_y = points[i];"\
            "}"\
        "}"\
        "window.onload = drawSpecter;"\
    "</script>"\
"</body>"\
"</html>"\

