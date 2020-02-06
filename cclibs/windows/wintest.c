
#include "windows.h"

int main(int argc, char **argv) {

    int ch;
	char text[] = "This is a test to see if this works. We are trying to \
increase the size of the line so that it will be forced to wrap onto the \
next line.\n\nThis should be on the next line.";
    Windows *win;

    win_init();

	win = win_open("Testing", 10, 30, 10 ,10, FALSE);
	ch = win_getch(win);

    ch = win_alert("Alert Testing", NULL, 4, text);


    win_quit();

    return 0;
    
}

