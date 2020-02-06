
#include "windows.h"

int fake(void);
int prtdemo(void);
int printer(void);
int lpt1(void);
int lpt2(void);
int text(void);

int main(void) {

	struct pop_struct pop_menu[6] = {
		/* name    (*fun)()    select_id  speed_key*/
		" Sort    ", fake,       0,          'S',
		" Print   ", prtdemo,    0,          'P', 
		" Delete  ", fake,       0,          'D',
		" Copy    ", fake,       0,          'C',
		" Quit    ", NULL,       1,          'Q',
		"\0"
		};

	int code;

	win_init();

	code = win_popup_menu(pop_menu, 33, 10, A_NORMAL, A_REVERSE);

	win_quit();

}

int prtdemo(void) {

	struct pop_struct pop_menu[3] = {
		" Printer   ", printer, 0, 'P',
		" Text-file ", text,    0, 'T',
		"\0"
		};
	int code;

	code = win_popup_menu(pop_menu, 31, 13, A_NORMAL, A_REVERSE);

	return(0);

}

int printer(void) {

	struct pop_struct pop_menu[3] = {
		" Lpt1:   ", lpt1, 0, '1',
		" lpt2:   ", lpt2, 0, '2',
		"\0"
		};

	return(win_popup_menu(pop_menu, 53, 15, A_NORMAL, A_REVERSE));

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

