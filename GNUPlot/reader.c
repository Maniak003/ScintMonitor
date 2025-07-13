
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>

#define BUFFER_SIZE 1026
#define PLOT_FILE			"mainChart-1.gp"
#define COLOR_LIN			"color_lin=\"0x80FF0000\"\n"
#define COLOR_LOG			"color_log=\"0x800000FF\"\n"
#define COLOR_TEXT			"color_text=\"0x00808080\"\n"
#define COLOR_FONE			"color_fone=\"0xC0C0A0\"\n"
//#define TITLE_STYLE		"set title \"AlphaSpecter. Pulses: %lu, Time: %lu sec, CPS: %f\" font \"Helvetica Bold, 10\" textcolor rgb color_text\n"
#define TITLE_STYLE			"set title \"GammaSpecter. Pulses: %lu (%lu), Time: %lu sec, CPS: %f\" textcolor rgb color_text\n"
#define LABEL_X				"set xlabel \"Channels\" textcolor rgb color_text\n"
#define LABEL_Y				"set ylabel \"Counts\" textcolor rgb color_text\n"
#define GRID_STYLE			"set grid lc rgb color_text nomxtics xtics ytics mytics\n"
#define BORDER_STYLE		"set border 3 lc rgb color_text\n"
#define X_TICS_STYLE		"set xtics nomirror\n"
#define Y_TICS_STYLE		"set ytics nomirror\n"
#define BACKGROUND_STYLE	"set term wxt background rgb color_fone\n"
#define RANGE_Y				"set yrange [0:%u]\nset y2range [0:%f]\n"
#define RANGE_X				"set xrange [0:%u]\n"
#define PLOT_STYLE			"plot '%s' using 1:2 with lines lc rgb color_lin notitle, '%s' using 1:($2==0 ? 0 : log($2)) axes x1y2 with lines lc rgb color_log notitle\n"
//#define PLOT_STYLE		"plot 'alpha.dat' using 1:2 with lines lc rgb color_lin title \"line\", 'alpha.dat' using 1:($2==0 ? 0 : log($2)) axes x1y2 with lines lc rgb color_log title \"log\"\n"
#define MTICS_X 			"set mxtics 10\n"
#define USAGE_TEXT			"Usage:%s\t{-u, -m} <serial port> <file name>\n\t\t-c <serial port>\n"
#define START_UPLOAD_CHANNEL 8


char receive_buf[BUFFER_SIZE], send_buf[3], string_buf[200];
uint32_t spectr_data[BUFFER_SIZE + 1] = {0,}, real_pulse = 0;
int hpio, nbyte, data_len, startFlag, fd, fd1;
struct termios ti;

void main(int argc, char *argv[]) {
	if (argc > 2) {
		if((hpio = open(argv[2], O_RDWR)) <= 0) {
			printf("Error open port: %s \n", argv[2]);
		} else {
			/*
			 * 	Настройка tty интерфейса
			 */
			tcgetattr(hpio, &ti);
			ti.c_lflag&=(~(ICANON | ECHO | ISIG | TOSTOP));
			//ti.c_cc[VMIN] = 0;
			//ti.c_cc[VTIME] = 3;
			//ti.c_cc[VLNEXT] = 0;
			//ti.c_lflag = 0;
			ti.c_cc[NCCS] = (~(VINTR | VQUIT | VERASE | VKILL | VEOF | VEOL | VEOL2 | VSTART\
				 | VSTOP | VSUSP | VREPRINT | VDISCARD | VWERASE | VLNEXT));
			ti.c_iflag &= ~(ICRNL | IXON | IXOFF);
			ti.c_cflag = (ti.c_cflag & ~CBAUD ) | B115200;
			tcsetattr(hpio, TCSANOW, &ti);

			if (argc == 3) {
				if (strstr(argv[1], "-c" )) {	// Запрос на очистку данных
				printf("Clear data.\n");
					send_buf[0] = 'c';
					write(hpio, send_buf, 1);
				}
			} else if ((argc == 4) && (strstr(argv[1], "-u" ) || strstr(argv[1], "-m"))) {
				printf("Start upload data.\n");
				/*
				 * Запрос занных у спектрометра
				 */
				send_buf[0] = 'b';
				write(hpio, send_buf, 1);

				/*
				 * Прием данных
				 */
				startFlag = 0;
				int idx = 0, start_seq = 0, odd_even_flag = 0;
				uint32_t cs_summ = 0;
				while (idx <= BUFFER_SIZE) {
					if((nbyte = read(hpio, receive_buf, sizeof(receive_buf))) > 0) {
						for ( int i = 0; i < nbyte; i++ ) {
							if ((start_seq == 0) && (receive_buf[i] == '<')) {		// Начало передачи данных
								start_seq++;
							} else if ((start_seq == 1) && (receive_buf[i] == 'B')) {
								start_seq++;
							} else if ((start_seq == 2) && (receive_buf[i] == '>')) {
								start_seq++;
							} else if (start_seq == 3) {	// Маркер найден, можно записывать данные
								if (odd_even_flag == 0) {	// 1 байт
									odd_even_flag++;
									spectr_data[idx] = receive_buf[i] & 0xFF;
								} else if (odd_even_flag == 1) {	// 2 байт
									odd_even_flag++;
									spectr_data[idx] = spectr_data[idx] | (uint32_t) (receive_buf[i] << 8) & 0xFF00;
								} else if (odd_even_flag == 2) {	// 3 байт
									odd_even_flag++;
									spectr_data[idx] = spectr_data[idx] | (uint32_t) (receive_buf[i] << 16) & 0xFF0000;
								} else if (odd_even_flag == 3) {							// 4 байт
									odd_even_flag = 0;
									spectr_data[idx] = spectr_data[idx] | ((uint32_t) receive_buf[i] << 24 & 0xFF000000);
									idx++;
									if (idx > BUFFER_SIZE) {
										printf("Specter bufer complete. Idx: %d.\n", idx);
										break;
									}
								}
							}
						}
					}
				}
				/* Проверка контрольной суммы */
				for (int i = 0; i < BUFFER_SIZE; i++) {
					cs_summ = cs_summ + spectr_data[i];
				}
				if ( 0 || (cs_summ == spectr_data[BUFFER_SIZE])) {		// Контрольная сумма совпала, формируем данные
					/* Выгрузка данных в файл */
					printf("CS matched: %u\nUpload data file %s.\n", cs_summ, argv[3]);
					fd = open(argv[3], O_RDWR | O_CREAT | O_TRUNC, S_IRWXU);
					uint32_t Amp, Am[3] = {0,}, cnt = 0;
					uint32_t max_lin = 0;
					double tmp_log,  max_log = 0;
					for (int i = START_UPLOAD_CHANNEL; i < BUFFER_SIZE; i++) {
						bzero(string_buf, sizeof(string_buf));
						if (strstr(argv[1], "-m")) {			// Медианный фильтр
							Am[cnt] = spectr_data[i];
							if (++cnt >= 3) cnt = 0;
							Amp = (Am[0] < Am[1]) ? ((Am[1] < Am[2]) ? Am[1] : ((Am[2] < Am[0]) ? Am[0] : Am[2])) : ((Am[0] < Am[2]) ? Am[0] : ((Am[2] < Am[1]) ? Am[1] : Am[2]));
						} else {
							Amp = spectr_data[i];
						}
						real_pulse = real_pulse + Amp;
						sprintf(string_buf, "%d %u\n", i, Amp);
						write(fd, string_buf, strlen(string_buf));
						//printf(string_buf);
						
						/* Находим диапазон для линейного графика */
						if (max_lin < Amp) {
							max_lin = Amp;
						}
						
						/* Находим диапазон для логарифмического графика */
						if (Amp > 0) {
							tmp_log = log((double) Amp);
							if (max_log < tmp_log) {
								max_log = tmp_log;
							}
						}
					}

					uint32_t messur = spectr_data[0];
					uint32_t pulses = spectr_data[1];
					float cps = (float) pulses / messur;
					printf("MAX_LIN: %u\nMAX_LOG: %f\nPulses: %lu\nRealPulse: %lu\nTime: %lu\nCPS: %f\n", max_lin, max_log, pulses, real_pulse, messur, cps);
					/* Формирование gnuplot скрипта */
					printf("Make control file %s.\n", PLOT_FILE);
					fd1 = open(PLOT_FILE, O_RDWR | O_CREAT | O_TRUNC, S_IRWXU);
					write(fd1, COLOR_LIN, strlen(COLOR_LIN));					// Цвет линейного графика
					write(fd1, COLOR_LOG, strlen(COLOR_LOG));					// Цвет логирифмического графика
					write(fd1, COLOR_TEXT, strlen(COLOR_TEXT));					// Цвет текста
					write(fd1, COLOR_FONE, strlen(COLOR_FONE));					// Цвет фона
					
					bzero(string_buf, sizeof(string_buf));
					sprintf(string_buf, TITLE_STYLE, pulses, real_pulse, messur, cps);		// Подготовка заголовка графика
					write(fd1, string_buf, strlen(string_buf));					// Заголовок графика
					write(fd1, LABEL_X, strlen(LABEL_X));						// Название оси X
					write(fd1, LABEL_Y, strlen(LABEL_Y));						// Название оси Y
					write(fd1, MTICS_X, strlen(MTICS_X));						// Разбивка шкалы оси X
					write(fd1, GRID_STYLE, strlen(GRID_STYLE));					// Стиль сетки графика
					write(fd1, BORDER_STYLE, strlen(BORDER_STYLE));				// Стиль осей графика
					write(fd1, X_TICS_STYLE, strlen(X_TICS_STYLE));				// Стиль оси X
					write(fd1, Y_TICS_STYLE, strlen(Y_TICS_STYLE));				// Стиль оси Y
					write(fd1, BACKGROUND_STYLE, strlen(BACKGROUND_STYLE));		// Фон графика
					if (max_log == 0) {
						max_log = 1;
					}
					bzero(string_buf, sizeof(string_buf));
					sprintf(string_buf, RANGE_X, BUFFER_SIZE);					// Диапазон по оси X
					write(fd1, string_buf, strlen(string_buf));					//
					bzero(string_buf, sizeof(string_buf));
					sprintf(string_buf, RANGE_Y, max_lin, max_log);				// Диапазон для линейного и логарифмического графиков
					write(fd1, string_buf, strlen(string_buf));					//
					bzero(string_buf, sizeof(string_buf));
					sprintf(string_buf, PLOT_STYLE, argv[3], argv[3]);			// Рисование графиков
					write(fd1, string_buf, strlen(string_buf));					//
					close(fd1);
					
					close(fd);
					close(hpio);
					printf("Execute gnuplot.\n");
					execlp("gnuplot", " ", "-p", PLOT_FILE, NULL);
				} else {
					printf("CS ERROR. Calculate CS: %u, upload CS: %u, Time: %u, Pulses: %u, Idx: %d.\n", cs_summ, spectr_data[BUFFER_SIZE], spectr_data[0], spectr_data[1], idx);
				}
			} else {
				printf(USAGE_TEXT, argv[0]);
			}
		}
	} else {
		printf(USAGE_TEXT, argv[0]);
	}
}

