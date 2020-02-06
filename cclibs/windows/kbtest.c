
#include <stdio.h>
#include "windows.h"

int main(void) {
   
	int ch;
	char buff[100];
	Windows *win;
   
	win_init();
	win = win_open("kbtest", 10, 30, 10 ,10, FALSE);
   
	for (;;) {
		
		ch = win_getch(win);
		sprintf(buff, "key pressed %o  ", ch);
		win_output(win, 1, 1, buff);
		
	}
   
	win_quit();
   
}
   
