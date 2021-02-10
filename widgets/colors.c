
/*---------------------------------------------------------------------------*/
/*                Copyright (c) 2019 by Kevin L. Esteb                       */
/*                                                                           */
/*  Permission to use, copy, modify, and distribute this software and its    */
/*  documentation for any purpose and without fee is hereby granted,         */
/*  provided that the above copyright notice appears in all copies. The      */
/*  author makes no representations about the suitability of this software   */
/*  for any purpose. It is provided "as is" without express or implied       */
/*  warranty.                                                                */
/*---------------------------------------------------------------------------*/

/*
 * Taken from: https://www.linuxjournal.com/content/about-ncurses-colors-0
 * With modifications.
 * 
 * The article is incorrect about the origination of the 8 
 * foreground/background colors. One of the original purposes of the IBM
 * PC was to replace the IBM 3270, a terminal for main frame computers. So the 
 * color palette would have to be compatible. Also during this time, various 
 * sanctioning bodies where codifying ASCII terminal escape sequences, which
 * included the color palette. Each terminal manufacture would have a different
 * set of escape sequences to toggle features. It was a nightmare. And to be
 * fair the 8-bit home computers didn't have more colors available.
 * 
 * On Unix this lead to the termcap/terminfo terminal capabilities databases 
 * and the curses library for manipulating the terminal interface, in a 
 * somewhat portable way. 
 * 
 * VMS had the SMG library to do the same thing. And VMS has a homogeneous 
 * terminal environment! There is a terminal capabilities database
 * (sys$system:termtable.exe). And it is expandable (sys$system:smgterms.txt).
 *
 * These libraries made life easier for the programmer. They also optimized 
 * writing to the terminal screen, if you ever lived on the end of a 2400 
 * baud terminal line, you would be very grateful for the optimization. They 
 * made using a text based terminal bearable. By the way, those optimization
 * also work on a GUI based terminal emulator. Think VNC/RDP for text 
 * terminals.
 * 
 * It is interesting to read blogs of newbies and some old timers, or for
 * that matter, Stack Exchange. Who say why don't you just write those escape 
 * sequences directly, it is so easy, just do blah blah blah, and here is 
 * this neat library that does blah blah blah for you.
 * 
 * Well that has been tried, 40 years ago, didn't work very well then and that 
 * is why the above libraries exist.
 * 
 */

#include <ncurses.h>

/*---------------------------------------------------------------------------*/

static int   _is_bold(int);
static short _curs_color(int);

/*---------------------------------------------------------------------------*/

int colornum(int fg, int bg) {

    /* must return a number between 0 and 63                                */
    /*                                                                      */
    /* this is a limitation on init_pair(), were the first parameter must   */
    /* be between 1 and COLOR_PAIRS - 1. color pair 0 is the terminals      */
    /* default forground/background color which is defined in the           */
    /* termcap/terminfo database. to override the terminals default colors, */
    /* you need to use the use_default_colors() function. this is not       */
    /* mentioned in the articule where these routines are adapted from.     */

    int B, bbb, ffff;

    B = 1 << 7;
    bbb = (7 & bg) << 3;
    ffff = 7 & fg;

    return ((B | bbb | ffff) - 128);

}

void init_colorpairs(void) {

    int bg = 0;
    int fg = 0;
    int colorpair = 0;

    start_color();
    use_default_colors();

    for (bg = 0; bg <= 7; bg++) {

        for (fg = 0; fg <= 7; fg++) {

            colorpair = colornum(fg, bg);
            init_pair(colorpair, _curs_color(fg), _curs_color(bg));

        }

    }

}

int wcoloron(WINDOW *win, int fg, int bg) {

    int stat = OK;

    /* set the color pair (colornum) and bold/bright (A_BOLD) */

    if (_is_bold(fg)) {

        stat = wattron(win, A_BOLD);
        if (stat == ERR) goto fini;

    }

    stat = wattron(win, COLOR_PAIR(colornum(fg, bg)));

    fini:
    return stat;

}

int wcoloroff(WINDOW *win, int fg, int bg) {

    int stat = OK;

    /* unset the color pair (colornum) and bold/bright (A_BOLD) */

    if (_is_bold(fg)) {

        stat = wattroff(win, A_BOLD);
        if (stat == ERR) goto fini;

    }

    stat = wattroff(win, COLOR_PAIR(colornum(fg, bg)));

    fini:
    return stat;

}

/*---------------------------------------------------------*/
/* private methods                                         */
/*---------------------------------------------------------*/

static short _curs_color(int fg) {
    
    int color = COLOR_BLACK;

    switch (7 & fg) {               /* RGB */
        case 0:                     /* 000 */
            color = COLOR_BLACK;
            break;
        case 1:                     /* 001 */
            color = COLOR_BLUE;
            break;
        case 2:                     /* 010 */
            color = COLOR_GREEN;
            break;
        case 3:                     /* 011 */
            color = COLOR_CYAN;
            break;
        case 4:                     /* 100 */
            color = COLOR_RED;
            break;
        case 5:                     /* 101 */
            color = COLOR_MAGENTA;
            break;
        case 6:                     /* 110 */
            color = COLOR_YELLOW;
            break;
        case 7:                     /* 111 */
            color = COLOR_WHITE;
            break;
    }

    return color;

}

static int _is_bold(int fg) {
    
    /* return the intensity bit */

    int i;

    i = 1 << 3;
    return (i & fg);

}

