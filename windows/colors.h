
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

#ifndef _COLORS_H
#define _COLORS_H

#include <ncurses.h>

/*-------------------------------------------------------------*/
/* color constants                                             */
/*-------------------------------------------------------------*/

#define BLACK    0      /* foreground/background */
#define BLUE     1
#define GREEN    2
#define CYAN     3
#define RED      4
#define MAGNETA  5
#define BROWN    6
#define WHITE    7
#define GRAY     8      /* foreground only, sets the A_BOLD attribute */
#define BBLUE    9
#define BGREEN   10
#define BCYAN    11
#define BRED     12
#define BMAGNETA 13
#define BYELLOW  14
#define BWHITE   15

/*-------------------------------------------------------------*/
/* interface                                                   */
/*-------------------------------------------------------------*/

#define setcolor(fg, bg)   wsetcolor((stdscr), (fg), (bg))
#define unsetcolor(fg, bg) wunsetcolor((stdscr), (fg), (bg))

extern int  colornum(int, int);
extern void init_colorpairs(void);
extern void wsetcolor(WINDOW *, int, int);
extern void wunsetcolor(WINDOW *, int, int);

#endif

