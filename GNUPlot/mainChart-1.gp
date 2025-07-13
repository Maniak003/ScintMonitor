color_lin="0x80FF0000"
color_log="0x800000FF"
color_text="0x00808080"
color_fone="0xC0C0A0"
set title "GammaSpecter. Pulses: 12941611 (12920096), Time: 2299 sec, CPS: 5629.234863" textcolor rgb color_text
set xlabel "Channels" textcolor rgb color_text
set ylabel "Counts" textcolor rgb color_text
set mxtics 10
set grid lc rgb color_text nomxtics xtics ytics mytics
set border 3 lc rgb color_text
set xtics nomirror
set ytics nomirror
set term wxt background rgb color_fone
set xrange [0:1026]
set yrange [0:141942]
set y2range [0:11.863174]
plot 'alpha-01072025.dat' using 1:2 with lines lc rgb color_lin notitle, 'alpha-01072025.dat' using 1:($2==0 ? 0 : log($2)) axes x1y2 with lines lc rgb color_log notitle
