
/*---------------------------------------------------------------------------*/
/*                Copyright (c) 2021 by Kevin L. Esteb                       */
/*                                                                           */
/*  Permission to use, copy, modify, and distribute this software and its    */
/*  documentation for any purpose and without fee is hereby granted,         */
/*  provided that this copyright notice appears in all copies. The author    */
/*  makes no representations about the suitability of this software for      */
/*  any purpose. It is provided "as is" without express or implied warranty. */
/*                                                                           */
/*---------------------------------------------------------------------------*/

#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <signal.h>

#include "status.h"

struct termios oldChars;
struct termios newChars;

int tty_raw(void) {

    tcgetattr(STDIN_FILENO, &oldChars);

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
            tty_reset();
            raise(SIGINT);    
            break;
        case 26:              /* ^Z */
            tty_reset();
            raise(SIGTSTP);
            break;
        case 28:              /* ^\ */
            tty_reset();
            raise(SIGTERM);
            break;
    }

    return ch;

}

