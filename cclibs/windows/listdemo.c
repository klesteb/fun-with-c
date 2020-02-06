
#include "windows.h"

int main(void) {

	char *ptr[] = { 
		"Apple      ", "Boat       ", "Berry      ",
        "Car        ", "Computer   ", "Denver     ",
		"Donut      ", "Dog        ", "Elephant   ",
		"Egg        ", "Fruit Cake ", "Food Bars  ",
        "Candy Bars ", "Rasins     ", "Junk Food  ",
		"Potatos    ", "Carrots    ", "Racers     ",
		"Trucks     ", "Planes     ", "Bikes      ",
		"Trains     ", '\0'
		};
	int select, ch;
	char buff[BUFSIZ];
	Windows *win;

	win_init();

	select = win_list_select(ptr, 2, 10, A_NORMAL, A_REVERSE);

	win = win_open("Result", 4, 20, 5, 5, FALSE);
	
	sprintf(buff, "Results = %d", select);

	win_output(win, 0, 1, buff);

	ch = win_getch(win);

	win_close(win);

	win_quit();

}


