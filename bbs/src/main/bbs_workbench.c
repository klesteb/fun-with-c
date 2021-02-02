
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
#include <locale.h>
#include <panel.h>
#include <signal.h>
#include <errno.h>

#include "when.h"
#include "colors.h"
#include "common.h"
#include "window.h"
#include "workbench.h"
#include "item_list.h"

require_klass(WORKBENCH_KLASS);

#ifndef CTRL
#define CTRL(c) ((c) & 037)
#endif

/*----------------------------------------------------------------*/
/* klass overrides                                                */
/*----------------------------------------------------------------*/

int _bbs_read_stdin(workbench_t *self) {

    int ch;
    int stat = OK;

    if ((ch = getch()) != ERR) {

        events_t *event = calloc(1, sizeof(events_t));

        if ((ch == KEY_RESIZE) || (ch == CTRL('w'))) {

            self->_refresh(self);
            update_panels();
            doupdate();

        } else if (ch == KEY_F(11)) {

            if ((self->panels > 0)) {

                PANEL *current = NULL;
                window_t *window = NULL;

                if ((current = panel_above(NULL)) != NULL) {

                    window = panel_userptr(current);
                    self->panel = current;

                    top_panel(current);
                    window_refresh(window);
                    update_panels();
                    doupdate();

                }

            }

        } else if (ch == KEY_F(12)) {

            self->dtor(OBJECT(self));
            raise(SIGTERM);

        } else {

            KEVENT *kevent = calloc(1, sizeof(KEVENT));
            kevent->keycode = ch;

            event->type = EVENT_K_KEYBOARD;
            event->data = (void *)kevent;

            stat = self->_queue_event(self, event);

        }

    }

    return stat;

}

int _bbs_init_terminal(workbench_t *self) {

    int stat = OK;

    when_error_in {

        setlocale(LC_ALL, "");

        /* initialize the terminal */

        errno = 0;
        if ((initscr() == NULL)) {

            cause_error(errno);

        }

        /* check for color capability */

        stat = has_colors();
        check_status(stat, TRUE, E_NOCOLOR);

        cbreak();
        noecho();
        init_colorpairs();
        keypad(stdscr, TRUE);
        nodelay(stdscr, TRUE);

        erase();
        refresh();
        curs_set(0);

        exit_when;

    } use {

        stat = ERR;

        object_set_error2(self, trace_errnum, trace_lineno, trace_filename, trace_function);
        clear_error();

    } end_when;

    return stat;

}

int _bbs_event(workbench_t *self, events_t *event) {

    int stat = ERR;
    window_t *window = NULL;

    if (self->panel != NULL) {

        window = (window_t *)panel_userptr(self->panel);
        stat = window_event(window, event);

        update_panels();

    }

    doupdate();

    return stat;

}

/*----------------------------------------------------------------*/
/* klass implementation                                           */
/*----------------------------------------------------------------*/

workbench_t *bbs_workbench_create(void) {

    item_list_t items[3];
    workbench_t *workbench = NULL;

    SET_ITEM(items[0], WORKBENCH_M_INIT_TERMINAL, _bbs_init_terminal, 0, NULL);
    SET_ITEM(items[1], 0, 0, 0, 0);

    if ((workbench = workbench_create(items))) {

        SET_ITEM(items[0], WORKBENCH_M_READ_STDIN, _bbs_read_stdin, 0, NULL);
        SET_ITEM(items[1], WORKBENCH_M_EVENT, _bbs_event, 0, NULL);
        SET_ITEM(items[2], 0, 0, 0, 0);

        workbench_override(workbench, items);

    }

    return workbench;

}

