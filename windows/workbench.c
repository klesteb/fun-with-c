
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

#include "when.h"
#include "colors.h"
#include "events.h"
#include "object.h"
#include "window.h"
#include "nix_util.h"
#include "que_util.h"
#include "container.h"
#include "workbench.h"
#include "error_codes.h"

#define TIMEOUT 1.0

require_klass(OBJECT_KLASS);

/*----------------------------------------------------------------*/
/* global variables                                               */
/*----------------------------------------------------------------*/

int pfd[2];                    /* self pipe fd's          */
NxInputId pipe_id;             /* id for pipe events      */
NxInputId stdin_id;            /* id for stdin events     */
NxIntervalId timeout_id;       /* id for timeout events   */
struct sigaction old_sigint;   /* ncurses sigint handler  */
struct sigaction old_sigterm;  /* ncurses sigterm handler */

/*----------------------------------------------------------------*/
/* klass methods                                                  */
/*----------------------------------------------------------------*/

int _workbench_ctor(object_t *, item_list_t *);
int _workbench_dtor(object_t *);

int _workbench_draw(workbench_t *);
int _workbench_loop(workbench_t *);
int _workbench_refresh(workbench_t *);
int _workbench_event(workbench_t *, event_t *);
int _workbench_get_focus(workbench_t *, window_t *);
int _workbench_set_focus(workbench_t *, window_t *);
int _workbench_compare(workbench_t *, workbench_t *);
int _workbench_add_window(workbench_t *, window_t *);
int _workbench_override(workbench_t *, item_list_t *);
int _workbench_inject_event(workbench_t *, event_t *);
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

int workbench_inject_event(workbench_t *self, event_t *event) {

    int stat = OK;

    when_error {

        if ((self != NULL) && (event != NULL)) {

            stat = self->_inject_event(self, event);
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

int workbench_loop(workbench_t *self) {

    int stat = OK;

    when_error {

        if (self != NULL) {

            stat = self->_loop(self);
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
/* private methods                                                */
/*----------------------------------------------------------------*/

static void _sig_handler(int sig) {

    int saved = errno;

    if ((sig == SIGINT) || (sig == SIGTERM)) {

        /* disable signal handling */

        struct sigaction act;
        sigemptyset(&act.sa_mask);
        act.sa_flags = 0;
        act.sa_handler = SIG_IGN;

        if (sigaction(sig, &act, NULL) == 0) {

            /* write signal to pipe */

            if (write(pfd[1], &sig, sizeof(int)) == -1 && errno != EAGAIN) {

                /* hmmm, what to do, what to do */

            }

        }

    }

    errno = saved;

}

static int _init_self_pipe(workbench_t *self) {

    int flags = 0;
    int stat = OK;
    struct sigaction sa;

    /* create our pipe */

    errno = 0;
    if (pipe(pfd) == -1) {

        stat = ERR;
        object_set_error(self, errno);
        goto fini;

    }

    /* Make read end nonblocking */

    errno = 0;
    if ((flags = fcntl(pfd[0], F_GETFL)) == -1) {

        stat = ERR;
        object_set_error(self, errno);
        goto fini;

    }

    errno = 0;
    flags |= O_NONBLOCK;                
    if (fcntl(pfd[0], F_SETFL, flags) == -1) {

        stat = ERR;
        object_set_error(self, errno);
        goto fini;

    }

    /* Make write end nonblocking */

    errno = 0;
    if ((flags = fcntl(pfd[1], F_GETFL)) == -1) {

        stat = ERR;
        object_set_error(self, errno);
        goto fini;

    }

    errno = 0;
    flags |= O_NONBLOCK;                
    if (fcntl(pfd[1], F_SETFL, flags) == -1) {

        stat = ERR;
        object_set_error(self, errno);
        goto fini;

    }

    /* ncurses installs it own signal handlers. these     */
    /* handle SIGWINCH, SIGINT, SIGTERM and SIGTSTP.      */
    /* please see ncurses/tty/lib_tstp.c for the gory     */
    /* details.                                           */
    /*                                                    */
    /* we would like to capture SIGINT and SIGTERM to     */
    /* perform resource cleanup. just using our own       */
    /* signal handlers lead to memory corruption and      */
    /* dumping core, so this work around was implemented. */
    /*                                                    */
    /* this is a shim to capture ncurses signal handlers  */
    /* so that we can hook in our own signal handlers to  */
    /* clean up resources. _read_pipe() reinstalls the    */
    /* saved signal handlers to allow normal ncurses      */
    /* signal handling. there are no provisions within    */
    /* ncurses to do this, so we get to play these        */
    /* games...                                           */
    /*                                                    */
    /* I wonder if Mr. Dickey would approve...            */
    /*                                                    */

    /* capture ncurses signal handlers */

    errno = 0;
    if ((sigaction(SIGTERM, NULL, &old_sigterm)) != 0) {

        stat = ERR;
        object_set_error(self, errno);
        goto fini;

    }

    errno = 0;
    if ((sigaction(SIGINT, NULL, &old_sigint)) != 0) {

        stat = ERR;
        object_set_error(self, errno);
        goto fini;

    }

    /* set up our signal handlers. */

    sigemptyset(&sa.sa_mask);
    sa.sa_flags |= SA_RESTART;
    sa.sa_handler = _sig_handler;

    errno = 0;
    if (sigaction(SIGINT, &sa, NULL) == -1) {

        stat = ERR;
        object_set_error(self, errno);
        goto fini;

    }

    errno = 0;
    if (sigaction(SIGTERM, &sa, NULL) == -1) {

        stat = ERR;
        object_set_error(self, errno);
        goto fini;

    }

    fini:
    return stat;

}

static int _read_pipe(NxAppContext context, NxInputId id, int source, void *data) {

    int sig;
    int size = 0;
    int stat = OK;
    workbench_t *self = (workbench_t *)data;

    for (;;) {                      /* Consume bytes from pipe */

        errno = 0;
        if ((size = read(pfd[0], &sig, sizeof(int)) > 0)) {

            if ((sig == SIGINT) || (sig == SIGTERM)) {

                /* preform cleanup */

                stat = self->dtor((object_t *)self);

                /* reinstall ncurses signal handlers */

                errno = 0;
                if (sigaction(SIGINT, &old_sigint, NULL) != 0) {

                    stat = ERR;
                    object_set_error(self, errno);
                    goto fini;

                }

                errno = 0;
                if (sigaction(SIGTERM, &old_sigterm, NULL) != 0) {

                    stat = ERR;
                    object_set_error(self, errno);
                    goto fini;

                }

                /* re-raise the signal, ncurses should now do  */
                /* it's own cleanup and cleanly exit.          */

                raise(sig);

            }

        } else {

            stat = ERR;
            object_set_error(self, errno);
            goto fini;

        }

    }

    fini:
    return stat;

}

static int _read_stdin(NxAppContext context, NxInputId id, int source, void *data) {

    int ch;
    int stat = OK;
    workbench_t *self = (workbench_t *)data;

    while ((ch = getch()) != ERR) {

        event_t *event = calloc(1, sizeof(event_t));

        if (ch == KEY_MOUSE) {

            MEVENT *mevent = calloc(1, sizeof(MEVENT));

            if ((getmouse(mevent) == OK)) {

                event->type = EVENT_K_MOUSE;
                event->data = (void *)mevent;

                stat = que_push_tail(&self->events, event);

            } else {

                free(mevent);

            }

        } else if (ch == KEY_RESIZE) {

            self->_refresh(self);

        } else if (ch == KEY_F(11)) {

            PANEL *current = NULL;
            window_t *window = NULL;

            current = panel_above(NULL);
            window = panel_userptr(current);
            self->panel = current;

            top_panel(current);
            window_refresh(window);
            update_panels();
            doupdate();

        } else if (ch == KEY_F(12)) {

            raise(SIGTERM);

        } else {

            KEVENT *kevent = calloc(1, sizeof(KEVENT));
            kevent->keycode = ch;

            event->type = EVENT_K_KEYBOARD;
            event->data = (void *)kevent;

            stat = que_push_tail(&self->events, event);

        }

    }

    return stat;

}

static int _event_handler(NxAppContext context, NxIntervalId id, void *data) {

    int stat = OK;
    event_t *event = NULL;
    workbench_t *self = (workbench_t *)data;

    while ((event = que_pop_head(&self->events))) {

        stat = self->_event(self, event);
        free(event->data);
        free(event);
        if (stat != OK) break;

    }

    if (que_empty(&self->events)) {

        que_init(&self->events);

    }

    timeout_id = NxAddTimeOut(NULL, TIMEOUT, _event_handler, (void *)self);

    return stat;

}

/*----------------------------------------------------------------*/
/* klass implementation                                           */
/*----------------------------------------------------------------*/

int _workbench_ctor(object_t *object, item_list_t *items) {

    int stat = ERR;
    workbench_t *self = NULL;

    if (object != NULL) {

        /* capture our items */

        if (items != NULL) {

            int x;
            for (x = 0;; x++) {

                if ((items[x].buffer_length == 0) &&
                    (items[x].item_code == 0)) break;

                /* switch(items[x].item_code) { */
                /*     case WORKBENCH_K_TYPE: { */
                /*         memcpy(&type,  */
                /*                items[x].buffer_address,  */
                /*                items[x].buffer_length); */
                /*         break; */
                /*     } */
                /* } */

            }

        }

        /* initilize our base klass here */

        object_set_error(object, OK);

        /* initialize our derived klass here */

        self = WORKBENCH(object);

        /* assign our methods here */

        self->ctor = _workbench_ctor;
        self->dtor = _workbench_dtor;
        self->_compare = _workbench_compare;
        self->_override = _workbench_override;

        self->_draw = _workbench_draw;
        self->_loop = _workbench_loop;
        self->_event = _workbench_event;
        self->_refresh = _workbench_refresh;
        self->_get_focus = _workbench_get_focus;
        self->_set_focus = _workbench_set_focus;
        self->_add_window = _workbench_add_window;
        self->_inject_event = _workbench_inject_event;
        self->_remove_window = _workbench_remove_window;

        /* initialize internal variables here */

        que_init(&self->events);

        self->panel = NULL;

        /* initialize the terminal */

        errno = 0;
        if ((slk_init(1) != OK)) {

            object_set_error(self, errno);
            goto fini;

        }

        errno = 0;
        if ((initscr() == NULL)) {

            object_set_error(self, errno);
            goto fini;

        }

        /* check for color capability */

        if (has_colors() == FALSE) {

            object_set_error(self, E_NOCOLOR);
            goto fini;

        }

        cbreak();
        noecho();
        init_colorpairs();
        keypad(stdscr, TRUE);
        nodelay(stdscr, TRUE);
        mousemask(ALL_MOUSE_EVENTS, NULL);
        slk_noutrefresh();

        erase();
        refresh();
        curs_set(0);

        /* create a "self pipe" for signal handling */
        /* this must run after the initscr() call   */

        stat = _init_self_pipe(self);

    }

    fini:
    return stat;

}

int _workbench_dtor(object_t *object) {

    int stat = OK;
    PANEL *panel = NULL;
    event_t *event = NULL;
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

    endwin();

    if (isendwin()) {

        NxRemoveInput(NULL, pipe_id);
        NxRemoveInput(NULL, stdin_id);
        NxRemoveTimeOut(NULL, timeout_id);

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
                case WORKBENCH_M_LOOP: {
                    self->_loop = items[x].buffer_address;
                    stat = OK;
                    break;
                }
                case WORKBENCH_M_INJECT_EVENT: {
                    self->_inject_event = items[x].buffer_address;
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
        (self->_loop == other->_loop) &&
        (self->_event == other->_event) &&
        (self->_refresh == other->_refresh) &&
        (self->_get_focus == other->_get_focus) &&
        (self->_set_focus == other->_set_focus) &&
        (self->_add_window == other->_add_window) &&
        (self->_inject_event == other->_inject_event) &&
        (self->_remove_window == other->_remove_window)) {

        stat = OK;

    }

    return stat;

}

int _workbench_event(workbench_t *self, event_t *event) {

    int stat = ERR;
    window_t *window = NULL;

    if (self->panel != NULL) {

        window = (window_t *)panel_userptr(self->panel);
        stat = window_event(window, event);
        update_panels();
        doupdate();

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

        window = (window_t *)panel_userptr(panel);
        stat = window_refresh(window);
        if (stat != OK) break;

    }

    update_panels();
    doupdate();

    return stat;

}

int _workbench_inject_event(workbench_t *self, event_t *event) {

    int stat = ERR;

    stat = que_push_tail(&self->events, event);

    return stat;

}

int _workbench_add_window(workbench_t *self, window_t *window) {

    int stat = ERR;
    PANEL *panel = NULL;

    if ((panel = new_panel(window->outer)) != NULL) {

        stat = OK;
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

int _workbench_loop(workbench_t *self) {

    int stat = OK;

    self->_draw(self);

    timeout_id = NxAddTimeOut(NULL, TIMEOUT, _event_handler, (void *)self);
    pipe_id = NxAddInput(NULL, pfd[0], NxInputReadMask, _read_pipe, (void *)self);
    stdin_id = NxAddInput(NULL, fileno(stdin), NxInputReadMask, _read_stdin, (void *)self);

    stat = NxMainLoop(NULL);

    return stat;

}

