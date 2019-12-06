
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
 */

#include <ncurses.h>

/*---------------------------------------------------------------------------*/

static int is_bold(int);
static int colornum(int, int);
static short xcurs_color(int);

/*---------------------------------------------------------------------------*/

void init_colorpairs(void) {
    
    int fg, bg;
    int colorpair;

    start_color();
    
    for (bg = 0; bg <= 7; bg++) {

        for (fg = 0; fg <= 7; fg++) {

            colorpair = colornum(fg, bg);
            init_pair(colorpair, xcurs_color(fg), xcurs_color(bg));

        }

    }

}

int setcolor(int fg, int bg) {
    
    int attr = 0;
    
    /* set the color pair (colornum) and bold/bright (A_BOLD) */

    attr = COLOR_PAIR(colornum(fg, bg));

    if (is_bold(fg)) {

        attr = (attr | A_BOLD);

    }

    return attr;

}

int unsetcolor(int fg, int bg) {

    int attr = 0;

    /* unset the color pair (colornum) and bold/bright (A_BOLD) */

    attr = COLOR_PAIR(colornum(fg, bg));

    if (is_bold(fg)) {

        attr = (attr & A_BOLD);

    }

    return attr;

}

/*---------------------------------------------------------*/
/* private methods                                         */
/*---------------------------------------------------------*/

static short xcurs_color(int fg) {
    
    int color = COLOR_BLACK;

    switch (7 & fg) {           /* RGB */
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

static int colornum(int fg, int bg) {

    int B, bbb, ffff;

    B = 1 << 7;
    bbb = (7 & bg) << 4;
    ffff = 7 & fg;

    return (B | bbb | ffff);

}

static int is_bold(int fg) {
    
    /* return the intensity bit */

    int i;

    i = 1 << 3;
    return (i & fg);

}

