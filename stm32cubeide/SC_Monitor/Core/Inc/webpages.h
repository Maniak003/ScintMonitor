
#ifndef __WEBPAGES_H
#define __WEBPAGES_H

#define WEB_PAGE_1 "<!DOCTYPE html><html><head>"\
 	 	 	 	 	"<title>Gamma spectrometer</title>"\
 	 	 	 	 	"<style>canvas {border: 1px solid #000;background-color: #303030;}</style>"\
 	 	 	 	 	"</head><body>"\
 	 	 	 	 	 	 "<canvas id=\"myCanvas\" width=\"1030\" height=\"600\"></canvas>"\
						 "<script>"\
						 	 "const canvas = document.getElementById('myCanvas');"\
						 	 "const ctx = canvas.getContext('2d');"\
						 	 "function drawPoint(x, y, color = '#000', size = 5) {"\
						 	 	 "ctx.beginPath();"\
						 	 	 "ctx.fillStyle = color;"\
						 	 	 "ctx.arc(x, y, size, 0, Math.PI * 2);"\
						 	 	 "ctx.fill();"\
						 	 "}"\
						 	 "function drawLine(x1, y1, x2, y2, color = '#000', width = 2) {"\
						 	 	 "ctx.beginPath();"\
						 	 	 "ctx.strokeStyle = color;"\
						 	 	 "ctx.lineWidth = width;ctx.moveTo(x1, y1);"\
						 	 	 "ctx.lineTo(x2, y2);ctx.stroke();"\
						 	 "}"\
						 	 "function drawSpecter() {"\
						 	 	 "old_x = 0;"\
						 	 	 "old_y = 0;"\
						 	 	 "old_y_lg = 0;"\
						 	 	 "const points = ["

#define WEB_PAGE_2 	"];"\
					"const max_y = Math.max(...points);"\
					"const koeff = 600 / max_y;"\
					"const koefflg = 600 / Math.log(max_y);"\
					"for (let i = 0; i < points.length; i++) {"\
						"drawLine(old_x, 600 - old_y * koeff, i, 600 - points[i] * koeff, 'green', 1);"\
						"drawLine(old_x, 600 - old_y_lg * koefflg, i, 600 - Math.log(points[i]) * koefflg, 'yellow', 1);"\
						"old_x = i; "\
						"old_y = points[i];"\
						"old_y_lg = Math.log(points[i]);"\
					"}"\
				"}"\
				"window.onload = drawSpecter;"\
				"</script></body></html>"

#define LENGTH_WEB_PAGE_1 (sizeof(WEB_PAGE_1) - 1)
#define LENGTH_WEB_PAGE_2 (sizeof(WEB_PAGE_2) - 1)

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
