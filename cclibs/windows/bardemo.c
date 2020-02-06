
#include "windows.h"


int fake(void);
int prtdemo(void);
int printer(void);
int lpt1(void);
int lpt2(void);
int text(void);

int main(void) {

	struct bar_struct main_menu[6] = {
		"Sort",   "Sort the data file",       fake,    0, 'S',
		"Print",  "Printer, Text-file",       prtdemo, 0, 'P',
		"Delete", "Delete current data file", fake,    0, 'D',
		"Copy",   "Copy data file to backup", fake,    0, 'C',
		"Quit",   "Quit and return to OS",    NULL,    1, 'Q',
		"\0"
		};

	Windows *bar_window;
	int code;

	win_init();

	bar_window = win_open("", 4, 80, 0, 0, FALSE);

	code = win_bar_menu(bar_window, main_menu, A_NORMAL, A_REVERSE);

	win_close(bar_window);

	win_quit();

}

int prtdemo(void) {

	struct bar_struct print_menu[3] = {
		"Printer", "Lpt1: or Lpt2:", printer, 0, 'P',
		"Text-file", "Send output to a text file", text, 0, 'T',
		"\0"
		};

	Windows *print_window;
	int code;

	print_window = win_open("", 4, 80, 4, 0, FALSE);

	code = win_bar_menu(print_window, print_menu, A_NORMAL, A_REVERSE);

	win_close(print_window);

	return(0);

}

int printer(void) {

	struct bar_struct lpt_menu[3] = {
		"Lpt1:", "Route output to printer on LPT1:", lpt1, 0, '1',
		"Lpt2:", "Route output to printer on LPT2:", lpt2, 0, '2',
		"\0"
		};

	Windows *lpt_window;
	int code;

	lpt_window = win_open("", 4, 80, 6, 0, FALSE);

	code = win_bar_menu(lpt_window, lpt_menu, A_NORMAL, A_REVERSE);

	win_close(lpt_window);

	return(code);

}

int lpt1(void) {

	return(1);

}

int lpt2(void) {

	return(1);

}

int text(void) {

	return(1);

}

int fake(void) {

	return(0);

}

