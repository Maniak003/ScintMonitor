
color_lin="0x80FF0000"
color_log="0x800000FF"

set term wxt background rgb "0xC0C0A0"
set title "AlphaSpecter" font "Helvetica Bold, 10" textcolor rgb "0x00808080"

set xlabel "Channels" textcolor rgb "0x00808080"
set ylabel "Counts" textcolor rgb "0x00808080"
set mxtics 10
set grid lc rgb "0x00808080" nomxtics xtics ytics mytics
set border 3 lc rgb "0x00808080"
set xtics nomirror
set ytics nomirror
set xrange [0:4096]
set yrange [0:6000]
set y2range [0:10]
#set size 1,1
plot 'alpha.dat' using 1:2 with lines lc rgb color_lin title "lin", \
     'alpha.dat' using 1:($2==0 ? 0 : log($2)) axes x1y2 with lines lc rgb color_log title "log"
