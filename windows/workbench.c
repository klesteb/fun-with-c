
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

#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <ncurses.h>
#include <panel.h>
#include <fcntl.h>
#include <signal.h>
#include <locale.h>

#include "jobs.h"
#include "when.h"
#include "common.h"
#include "colors.h"
#include "events.h"
#include "object.h"
#include "window.h"
#include "nix_util.h"
#include "que_util.h"
#include "container.h"

#define TIMEOUT 1.0
#ifndef CTRL
#define CTRL(c) ((c) & 037)
#endif

require_klass(OBJECT_KLASS);

/*----------------------------------------------------------------*/
/* global variables                                               */
/*----------------------------------------------------------------*/

NxWorkProcId workproc_id;      /* id for event processor  */

/*----------------------------------------------------------------*/
/* klass methods                                                  */
/*----------------------------------------------------------------*/

int _workbench_ctor(object_t *, item_list_t *);
int _workbench_dtor(object_t *);
int _workbench_compare(workbench_t *, workbench_t *);
int _workbench_override(workbench_t *, item_list_t *);

int _workbench_draw(workbench_t *);
int _workbench_refresh(workbench_t *);
int _workbench_read_stdin(workbench_t *);
int _workbench_init_terminal(workbench_t *);
int _workbench_dispatch(workbench_t *, int *);
int _workbench_event(workbench_t *, events_t *);
int _workbench_get_focus(workbench_t *, window_t *);
int _workbench_set_focus(workbench_t *, window_t *);
int _workbench_add_window(workbench_t *, window_t *);
int _workbench_queue_event(workbench_t *, events_t *);
int _workbench_remove_window(workbench_t *, window_t *);

/*----------------------------------------------------------------*/
/* klass declaration                                              */
/*----------------------------------------------------------------*/

declare_klass(WORKBENCH_KLASS) {
    .size = KLASS_SIZE(workbench_t),
    .name = KLASS_NAME(workbench_t),
    .ctor = _workbench_ctor,
    .dtor = _workbench_dtor,
};

/*----------------------------------------------------------------*/
/* klass interface                                                */
/*----------------------------------------------------------------*/

workbench_t *workbench_create(item_list_t *items) {

    int stat = ERR;
    workbench_t *self = NULL;

    self = (workbench_t *)object_create(WORKBENCH_KLASS, items, &stat);

    return self;

}

int workbench_destroy(workbench_t *self) {

    int stat = OK;

    when_error {

        if (self != NULL) {

            if (object_assert(self, workbench_t)) {

                stat = self->dtor(OBJECT(self));
                check_status(stat, OK, E_INVOPS);

            } else {

                cause_error(E_INVOBJ);

            }

        } else {

            cause_error(E_INVPARM);

        }

        exit_when;

    } use {

        stat = ERR;

        object_set_error2(self, trace_errnum, trace_lineno, trace_filename, trace_function);
        clear_error();

    } end_when;

    return stat;

}

int workbench_override(workbench_t *self, item_list_t *items) {

    int stat = OK;

    when_error {

        if (self != NULL) {

            stat = self->_override(self, items);
            check_status(stat, OK, E_INVOPS);

        } else {

            cause_error(E_INVPARM);

        }

        exit_when;

    } use {

        stat = ERR;

        object_set_error2(self, trace_errnum, trace_lineno, trace_filename, trace_function);
        clear_error();

    } end_when;

    return stat;

}

int workbench_compare(workbench_t *us, workbench_t *them) {

    int stat = OK;

    when_error {

        if (us != NULL) {

            if (object_assert(them, workbench_t)) {

                stat = us->_compare(us, them);
                check_status(stat, OK, E_NOTSAME);

            } else {

                cause_error(E_INVOBJ);

            }

        } else {

            cause_error(E_INVPARM);

        }

        exit_when;

    } use {

        stat = ERR;

        object_set_error2(us, trace_errnum, trace_lineno, trace_filename, trace_function);
        clear_error();

    } end_when;

    return stat;

}

int workbench_inject_event(workbench_t *self, events_t *event) {

    int stat = OK;

    when_error {

        if ((self != NULL) && (event != NULL)) {

            stat = self->_queue_event(self, event);
            check_status(stat, OK, E_INVOPS);

        } else {

            cause_error(E_INVPARM);

        }

        exit_when;

    } use {

        stat = ERR;

        object_set_error2(self, trace_errnum, trace_lineno, trace_filename, trace_function);
        clear_error();

    } end_when;

    return stat;

}

int workbench_add_window(workbench_t *self, window_t *window) {

    int stat = OK;

    when_error {

        if ((self != NULL) && (window != NULL)) {

            stat = self->_add_window(self, window);
            check_status(stat, OK, E_INVOPS);

        } else {

            cause_error(E_INVPARM);

        }

        exit_when;

    } use {

        stat = ERR;

        object_set_error2(self, trace_errnum, trace_lineno, trace_filename, trace_function);
        clear_error();

    } end_when;

    return stat;

}

int workbench_remove_window(workbench_t *self, window_t *window) {

    int stat = OK;

    when_error {

        if ((self != NULL) && (window != NULL)) {

            stat = self->_remove_window(self, window);
            check_status(stat, OK, E_INVOPS);

        } else {

            cause_error(E_INVPARM);

        }

        exit_when;

    } use {

        stat = ERR;

        object_set_error2(self, trace_errnum, trace_lineno, trace_filename, trace_function);
        clear_error();

    } end_when;

    return stat;

}

int workbench_get_focus(workbench_t *self, window_t *window) {

    int stat = OK;

    when_error {

        if ((self != NULL) && (window != NULL)) {

            stat = self->_get_focus(self, window);
            check_status(stat, OK, E_INVOPS);

        } else {

            cause_error(E_INVPARM);

        }

        exit_when;

    } use {

        stat = ERR;

        object_set_error2(self, trace_errnum, trace_lineno, trace_filename, trace_function);
        clear_error();

    } end_when;

    return stat;

}

int workbench_set_focus(workbench_t *self, window_t *window) {

    int stat = OK;

    when_error {

        if ((self != NULL) && (window != NULL)) {

            stat = self->_set_focus(self, window);
            check_status(stat, OK, E_INVOPS);

        } else {

            cause_error(E_INVPARM);

        }

        exit_when;

    } use {

        stat = ERR;

        object_set_error2(self, trace_errnum, trace_lineno, trace_filename, trace_function);
        clear_error();

    } end_when;

    return stat;

}

int workbench_refresh(workbench_t *self) {

    int stat = OK;

    when_error {

        if (self != NULL) {

            stat = self->_refresh(self);
            check_status(stat, OK, E_INVOPS);

        } else {

            cause_error(E_INVPARM);

        }

        exit_when;

    } use {

        stat = ERR;

        object_set_error2(self, trace_errnum, trace_lineno, trace_filename, trace_function);
        clear_error();

    } end_when;

    return stat;

}

int workbench_capture(workbench_t *self) {

    int stat = OK;

    when_error {

        if (self != NULL) {

            stat = self->_read_stdin(self);
            check_status(stat, OK, E_INVOPS);

        } else {

            cause_error(E_INVPARM);

        }

        exit_when;

    } use {

        stat = ERR;

        object_set_error2(self, trace_errnum, trace_lineno, trace_filename, trace_function);
        clear_error();

    } end_when;

    return stat;

}

int workbench_dispatch(workbench_t *self, int *again) {

    int stat = OK;

    when_error {

        if (self != NULL) {

            stat = self->_dispatch(self, again);
            check_status(stat, OK, E_INVOPS);

        } else {

            cause_error(E_INVPARM);

        }

        exit_when;

    } use {

        stat = ERR;

        object_set_error2(self, trace_errnum, trace_lineno, trace_filename, trace_function);
        clear_error();

    } end_when;

    return stat;

}

/*----------------------------------------------------------------*/
/* klass implementation                                           */
/*----------------------------------------------------------------*/

int _workbench_ctor(object_t *object, item_list_t *items) {

    int stat = ERR;
    workbench_t *self = NULL;
    int (*init_terminal)(workbench_t *) = _workbench_init_terminal;

    if (object != NULL) {

        /* capture our items */

        if (items != NULL) {

            int x;
            for (x = 0;; x++) {

                if ((items[x].buffer_length == 0) &&
                    (items[x].item_code == 0)) break;

                switch(items[x].item_code) {
                    case WORKBENCH_M_INIT_TERMINAL: {
                        init_terminal = items[x].buffer_address;
                        break;
                    }
                }

            }

        }

        /* initilize our base klass here */

        object_set_error1(object, OK);

        /* initialize our derived klass here */

        self = WORKBENCH(object);

        /* assign our methods here */

        self->ctor = _workbench_ctor;
        self->dtor = _workbench_dtor;
        self->_compare = _workbench_compare;
        self->_override = _workbench_override;

        self->_draw = _workbench_draw;
        self->_event = _workbench_event;
        self->_refresh = _workbench_refresh;
        self->_init_terminal = init_terminal;
        self->_dispatch = _workbench_dispatch;
        self->_get_focus = _workbench_get_focus;
        self->_set_focus = _workbench_set_focus;
        self->_read_stdin = _workbench_read_stdin;
        self->_add_window = _workbench_add_window;
        self->_queue_event = _workbench_queue_event;
        self->_remove_window = _workbench_remove_window;

        /* initialize internal variables here */

        when_error_in {

            que_init(&self->events);

            self->panels = 0;
            self->panel = NULL;

            stat = self->_init_terminal(self);
            check_return(stat, self);

            exit_when;

        } use {

            stat = ERR;

            object_set_error2(self, trace_errnum, trace_lineno, trace_filename, trace_function);
            clear_error();

        } end_when;

    }

    return stat;

}

int _workbench_dtor(object_t *object) {

    int stat = OK;
    PANEL *panel = NULL;
    events_t *event = NULL;
    window_t *window = NULL;
    workbench_t *self = WORKBENCH(object);

    /* free local resources here */

    while ((event = que_pop_head(&self->events))) {

        free(event->data);
        free(event);

    }

    if (que_empty(&self->events)) {

        que_init(&self->events);

    }

    while ((panel = panel_above(NULL))) {

        window = (window_t *)panel_userptr(panel);
        window_destroy(window);
        del_panel(panel);

    }

    werase(self->messages);
    delwin(self->messages);
    endwin();

    if (isendwin()) {

        NxRemoveWorkProc(NULL, workproc_id);

    }

    /* walk the chain, freeing as we go */

    object_demote(object, object_t);
    object_destroy(object);

    return stat;

}

int _workbench_override(workbench_t *self, item_list_t *items) {

    int stat = ERR;

    if (items != NULL) {

        int x;
        for (x = 0;; x++) {

            if ((items[x].buffer_length == 0) &&
                (items[x].item_code == 0)) break;

            switch(items[x].item_code) {
                case WORKBENCH_M_DESTROY: {
                    self->dtor = items[x].buffer_address;
                    stat = OK;
                    break;
                }
                case WORKBENCH_M_DRAW: {
                    self->_draw = items[x].buffer_address;
                    stat = OK;
                    break;
                }
                case WORKBENCH_M_EVENT: {
                    self->_event = items[x].buffer_address;
                    stat = OK;
                    break;
                }
                case WORKBENCH_M_REFRESH: {
                    self->_refresh = items[x].buffer_address;
                    stat = OK;
                    break;
                }
                case WORKBENCH_M_ADD_WINDOW: {
                    self->_add_window = items[x].buffer_address;
                    stat = OK;
                    break;
                }
                case WORKBENCH_M_REMOVE_WINDOW: {
                    self->_remove_window = items[x].buffer_address;
                    stat = OK;
                    break;
                }
                case WORKBENCH_M_SET_FOCUS: {
                    self->_set_focus = items[x].buffer_address;
                    stat = OK;
                    break;
                }
                case WORKBENCH_M_GET_FOCUS: {
                    self->_get_focus = items[x].buffer_address;
                    stat = OK;
                    break;
                }
                case WORKBENCH_M_READ_STDIN: {
                    self->_read_stdin = items[x].buffer_address;
                    stat = OK;
                    break;
                }
                case WORKBENCH_M_INIT_TERMINAL: {
                    self->_init_terminal = items[x].buffer_address;
                    stat = OK;
                    break;
                }
                case WORKBENCH_M_DISPATCH: {
                    self->_dispatch = items[x].buffer_address;
                    stat = OK;
                    break;
                }
            }

        }

    }

    return stat;

}

int _workbench_compare(workbench_t *self, workbench_t *other) {

    int stat = ERR;

    if ((object_compare(OBJECT(self), OBJECT(other)) == 0) &&
        (self->ctor == other->ctor) &&
        (self->dtor == other->dtor) &&
        (self->_compare == other->_compare) &&
        (self->_override == other->_override) &&
        (self->_draw == other->_draw) &&
        (self->_event == other->_event) &&
        (self->_refresh == other->_refresh) &&
        (self->_dispatch == other->_dispatch) &&
        (self->_get_focus == other->_get_focus) &&
        (self->_set_focus == other->_set_focus) &&
        (self->_read_stdin == other->_read_stdin) &&
        (self->_add_window == other->_add_window) &&
        (self->_queue_event == other->_queue_event) &&
        (self->_init_terminal == other->_init_terminal) &&
        (self->_remove_window == other->_remove_window)) {

        stat = OK;

    }

    return stat;

}

int _workbench_draw(workbench_t *self) {

    int stat = ERR;
    PANEL *panel = NULL;
    window_t *window = NULL;

    for (panel = panel_above(NULL);
         panel != NULL;
         panel = panel_above(panel)) {

        window = (window_t *)panel_userptr(panel);
        stat = window_draw(window);
        if (stat != OK) break;

    }

    update_panels();
    doupdate();

    return stat;

}

int _workbench_refresh(workbench_t *self) {

    int stat = ERR;
    PANEL *panel = NULL;
    window_t *window = NULL;

    for (panel = panel_above(NULL);
         panel != NULL;
         panel = panel_above(panel)) {

fprintf(stderr, "found a panel\n");
        window = (window_t *)panel_userptr(panel);
fprintf(stderr, "window %p\n", window);
        stat = window_refresh(window);
fprintf(stderr, "stat %d\n", stat);
        if (stat != OK) break;

    }

    update_panels();
    doupdate();

    return stat;

}

int _workbench_add_window(workbench_t *self, window_t *window) {

    int stat = ERR;
    PANEL *panel = NULL;

    if ((panel = new_panel(window->outer)) != NULL) {

        stat = OK;
        self->panels++;
        set_panel_userptr(panel, (void *)window);

    }

    return stat;

}

int _workbench_remove_window(workbench_t *self, window_t *window) {

    int stat = ERR;
    PANEL *panel = NULL;
    window_t *temp = NULL;

    for (panel = panel_above(NULL);
         panel != NULL;
         panel = panel_above(panel)) {

        temp = (window_t *)panel_userptr(panel);

        if ((window_compare(window, temp)) == OK) {

            if (self->panels > 0) {

                self->panels--;

            }

            del_panel(panel);
            stat = window_destroy(temp);

            break;

        }

    }

    return stat;

}

int _workbench_set_focus(workbench_t *self, window_t *window) {

    int stat = ERR;
    PANEL *panel = NULL;
    window_t *temp = NULL;

    for (panel = panel_above(NULL);
         panel != NULL;
         panel = panel_above(panel)) {

        temp = (window_t *)panel_userptr(panel);

        if ((window_compare(window, temp)) == OK) {

            self->panel = panel;
            stat = top_panel(panel);

            break;

        }

    }

    return stat;

}

int _workbench_get_focus(workbench_t *self, window_t *window) {

    int stat = ERR;
    window_t *temp = NULL;

    if (self->panel != NULL) {

        stat = OK;
        temp = (window_t *)panel_userptr(self->panel);
        memcpy(window, temp, sizeof(window_t));

    }

    return stat;

}

int _workbench_init_terminal(workbench_t *self) {

    int stat = OK;
    int row, col;

    when_error_in {

        setlocale(LC_ALL, "");

        /* initialize the terminal */

        errno = 0;
        stat = slk_init(3);
        check_status(stat, OK, errno);

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
        mousemask(ALL_MOUSE_EVENTS, NULL);

        erase();
        refresh();
        curs_set(0);

        /* set default "soft keys" */

        slk_set(9,  "Redraw", 0);
        slk_set(10, "Menu", 0);
        slk_set(11, "Cycle", 0);
        slk_set(12, "Quit", 0);
        slk_noutrefresh();

        /* create the message window */

        getmaxyx(stdscr, row, col);
        self->messages = newwin(1, col, row - 1, 0);

        exit_when;

    } use {

        stat = ERR;

        object_set_error2(self, trace_errnum, trace_lineno, trace_filename, trace_function);
        clear_error();

    } end_when;

    return stat;

}

/*----------------------------------------------------------------*/
/* event processing                                               */
/*----------------------------------------------------------------*/

int _workbench_event(workbench_t *self, events_t *event) {

    int stat = ERR;
    int length = 0;
    char message[256];
    window_t *window = NULL;

    if (event->type == EVENT_K_MESSAGE) {

        length = strlen((char *)event->data) + 3;
        length = (length > 255) ? 255 : length; 
        memset(message, '\0', 256);
        snprintf(message, length, "* %s", (char *)event->data);

        werase(self->messages);
        mvwaddstr(self->messages, 0, 0, message);
        stat = wnoutrefresh(self->messages);

    } else if (self->panel != NULL) {

        window = (window_t *)panel_userptr(self->panel);
        stat = window_event(window, event);

        update_panels();

    }

    doupdate();

    return stat;

}

int _workbench_dispatch(workbench_t *self, int *again) {

    int stat = OK;
    events_t *event = NULL;

    *again = TRUE;

    if ((event = que_pop_head(&self->events))) {

        stat = self->_event(self, event);

        free(event->data);
        free(event);

        if (que_empty(&self->events)) {

            *again = FALSE;
            stat = que_init(&self->events);

        }

    }

    return stat;

}

int _workbench_queue_event(workbench_t *self, events_t *event) {

    int stat = OK;

    if (que_empty(&self->events)) {

        stat = que_init(&self->events);

    }

    if (stat == OK) {

        stat = que_push_tail(&self->events, event);

    }

    return stat;

}

int _workbench_read_stdin(workbench_t *self) {

    int ch;
    int stat = OK;

    if (que_empty(&self->events)) {

        stat = que_init(&self->events);

    }

    if ((ch = getch()) != ERR) {

        events_t *event = calloc(1, sizeof(events_t));

        if (ch == KEY_MOUSE) {

            MEVENT *mevent = calloc(1, sizeof(MEVENT));

            if ((getmouse(mevent) == OK)) {

                event->type = EVENT_K_MOUSE;
                event->data = (void *)mevent;

                stat = self->_queue_event(self, event);

            } else {

                free(mevent);

            }

        } else if ((ch == KEY_RESIZE) || (ch == CTRL('w'))) {

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

            if ((self->panels > 0)) {

                PANEL *current = NULL;
                window_t *window = NULL;

                window = panel_userptr(self->panel);
                self->_remove_window(self, window);
                update_panels();
                doupdate();

                if ((current = panel_below(NULL)) != NULL) {

                    self->panel = current;
                    
                }

            } else {

                self->dtor(OBJECT(self));
                raise(SIGTERM);
                
            }

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

