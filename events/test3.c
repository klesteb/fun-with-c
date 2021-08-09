
#include <stdio.h>
#include <errno.h>
#include <termios.h>
#include <unistd.h>
#include <signal.h>

#include "include/when.h"
#include "events/event.h"

event_t *temp = NULL;

struct termios oldChars;
struct termios newChars;

int tty_raw(void) {

    tcgetattr(STDIN_FILENO, &oldChars); /* grab old terminal i/o settings */

    newChars = oldChars;           /* make new settings same as old  */
    newChars.c_lflag = 0;          /* input mode                     */
    newChars.c_oflag = 0;          /* output mode                    */
    newChars.c_lflag &= ~ICANON;   /* disable buffered i/o           */
    newChars.c_lflag &= ~ECHO;     /* disable echo mode              */
    newChars.c_cc[VMIN] = 1;       /* minimum chars to wait for      */
    newChars.c_cc[VTIME] = 1;      /* minimum time to wait           */

    tcsetattr(STDIN_FILENO, TCSANOW, &newChars);

    return OK;

}

int tty_reset(void) {

    tcsetattr(STDIN_FILENO, TCSANOW, &oldChars);

    return OK;

}

int inkey(void) {

    int ch = 0;

    ch = getchar();

    /* the terminal is in raw mode */
    /* so lets regain some control */

    switch(ch) {
        case 3:               /* ^C */
            raise(SIGINT);    
            break;
        case 26:              /* ^Z */
            tty_reset();
            raise(SIGTSTP);
            break;
        case 28:              /* ^\ */
            raise(SIGTERM);
            break;
    }

    return ch;

}

int input(void *data) {

    int ch;

    ch = inkey();

    printf("ch = %d\n", ch);

    return OK;

}

int main(int argc, char **argv) {

    int stat = OK;

    when_error_in {

        tty_raw();

        temp = event_create();
        check_creation(temp);

        stat = event_register_input(temp, fileno(stdin), input, NULL);
        check_return(stat, temp);

        stat = event_loop(temp);
        check_return(stat, temp);

        exit_when;

    } use {

        printf("Error: %d, line: %d, file: %s, function: %s\n",
               trace_errnum, trace_lineno, trace_filename, trace_function);

        clear_error();

    } end_when;

    event_destroy(temp);
    tty_reset();

    return 0;

}

