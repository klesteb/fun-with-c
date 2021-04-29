
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

#include <ncurses.h>
#include <errno.h>

#include "when.h"
#include "colors.h"
#include "events.h"
#include "object.h"
#include "window.h"
#include "item_list.h"
#include "misc/misc.h"
#include "error_codes.h"
#include "components/more.h"
#include "components/yesno.h"
#include "components/hline.h"

/*----------------------------------------------------------------*/
/* klass overrides                                                */
/*----------------------------------------------------------------*/

/*----------------------------------------------------------------*/
/* klass implementation                                           */
/*----------------------------------------------------------------*/

window_t *query_window(char *title, char *label, int (*callback)(int, error_trace_t *), char *fmt, ...) {

    va_list ap;
    queue lines;
    int col = 0;
    int row = 0;
    int stat = OK;
    char buf[1024];
    int startx = 0;
    int starty = 0;
    int height = 6;
    int width  = 60;
    window_t *win = NULL;
    component_t *more = NULL;
    component_t *yesno = NULL;
    component_t *hline = NULL;
    char *value = "F7=Yes  F8=No";

    when_error_in {

        stat = que_init(&lines);
        check_status(stat, QUE_OK, E_INVOPS);

        memset(buf, '\0', 1024);

        va_start(ap, fmt);
        vsnprintf(buf, 1023, fmt, ap);
        va_end(ap);

        stat = wordwrap(buf, 48, &lines);
        check_status(stat, OK, E_INVOPS);

        startx = ((getmaxx(stdscr) - width) / 2);
        starty = ((getmaxy(stdscr) - height) / 2);

        win = window_create(title, startx, starty, height, width, TRUE);
        check_creation(win);

        more = more_create(win, 1, 1, 3, 58, 1, &lines);
        check_creation(more);

        row = height - 2;
        hline = hline_create(win, row, col, width);
        check_creation(hline);

        row = height - 1;
        col = ((width - strlen(value)) / 2);
        width = strlen(value) + 1;
        yesno = yesno_create(win, row, col, 1, width, 2, value, callback);
        check_creation(yesno);

        stat = window_add(win, more);
        check_return(stat, win);

        stat = window_add(win, hline);
        check_return(stat, win);

        stat = window_add(win, yesno);
        check_return(stat, win);

        exit_when;

    } use {

        object_set_error2(win, trace_errnum, trace_lineno, trace_filename, trace_function);
        clear_error();

    } end_when;

    return win;

}

