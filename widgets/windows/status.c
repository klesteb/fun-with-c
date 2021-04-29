
/*---------------------------------------------------------------------------*/
/*                Copyright (c) 2021 by Kevin L. Esteb                       */
/*                                                                           */
/*  Permission to use, copy, modify, and distribute this software and its    */
/*  documentation for any purpose and without fee is hereby granted,         */
/*  provided that the above copyright notice appears in all copies. The      */
/*  author makes no representations about the suitability of this software   */
/*  for any purpose. It is provided "as is" without express or implied       */
/*  warranty.                                                                */
/*---------------------------------------------------------------------------*/

#include <ncurses.h>

#include "when.h"
#include "theme.h"
#include "colors.h"
#include "window.h"
#include "component.h"
#include "item_list.h"
#include "error_codes.h"
#include "components/bell.h"
#include "components/text.h"

require_klass(WINDOW_KLASS);

/*----------------------------------------------------------------*/
/* klass overrides                                                */
/*----------------------------------------------------------------*/

/*----------------------------------------------------------------*/
/* klass implementation                                           */
/*----------------------------------------------------------------*/

window_t *status(char *title, int sound, char *fmt, ...) {

    va_list ap;
    int stat = OK;
    char buf[1024];
    int startx = 0;
    int starty = 0;
    int height = 3;
    int width  = 60;
    bell_t *bell = NULL;
    window_t *temp = NULL;
    component_t *text = NULL;
    theme_t theme = {
        .attribute = A_NORMAL,
        .foreground = RED,
        .background = BLACK
    };

    when_error_in {

        memset(buf, '\0', 1024);

        va_start(ap, fmt);
        vsnprintf(buf, 1023, fmt, ap);
        va_end(ap);

        startx = ((getmaxx(stdscr) - width) / 2);
        starty = ((getmaxy(stdscr) - height) / 2);

        if (!title) title = "Status";

        temp = window_create(title, startx, starty, height, width, TRUE);
        check_creation(temp);

        text = text_create(temp, 1, 1, starty - 2, buf, strlen(buf));
        check_creation(text);

        bell = bell_create(temp, sound);
        check_creation(bell);

        stat = window_add(temp, bell);
        check_return(stat, temp);

        stat = window_add(temp, text);
        check_return(stat, temp);

        stat = component_set_theme(text, &theme);
        check_return(stat, text);

        exit_when;

    } use {

        stat = ERR;
        object_set_error2(temp, trace_errnum, trace_lineno, trace_filename, trace_function);
        clear_error();

    } end_when;

    return temp;

}

