
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

#define setcolor(row, col)   wsetcolor((stdscr), (row), (col))
#define unsetcolor(row, col) wunsetcolor((stdscr), (row), (col))

extern int  colornum(int, int);
extern void wsetcolor(WINDOW *, int, int);
extern void wunsetcolor(WINDOW *, int, int);
extern void init_colorpairs(void);

#endif

