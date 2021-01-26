
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

#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <unistd.h>

#include "when.h"
#include "object.h"
#include "events.h"
#include "nix_util.h"
#include "que_util.h"
#include "item_list.h"
#include "error_codes.h"

require_klass(OBJECT_KLASS);

/*----------------------------------------------------------------*/
/* global variables                                               */
/*----------------------------------------------------------------*/

int pfd[2];                    /* self pipe fd's          */
NxInputId pipe_id;             /* id for pipe events      */
struct sigaction old_sigint;   /* old sigint handler      */
struct sigaction old_sigterm;  /* old sigterm handler     */

typedef struct _callback_s {
    int (*input)(void *data);
    int reque;
    double interval;
    void *data;
    events_t *self;
} callback_t;

/*----------------------------------------------------------------*/
/* klass methods                                                  */
/*----------------------------------------------------------------*/

int _events_ctor(object_t *, item_list_t *);
int _events_dtor(object_t *);
int _events_compare(events_t *, events_t *);
int _events_override(events_t *, item_list_t *);

int _events_loop(events_t *);
int _events_register_input(events_t *, int, int (*input)(void *), void *);
int _events_register_worker(events_t *, int, int (*input)(void *), void *);
int _events_register_timer(events_t *, int, double, int (*input)(void *), void *);

/*----------------------------------------------------------------*/
/* private klass methods                                          */
/*----------------------------------------------------------------*/

static int _init_self_pipe(events_t *);
static int _read_pipe(NxAppContext, NxInputId, int, void *);
static int _dispatch_timer(NxAppContext, NxIntervalId, void *);
static int _dispatch_worker(NxAppContext, NxWorkProcId, void *);
static int _dispatch_input(NxAppContext, NxInputId, int, void *);

/*----------------------------------------------------------------*/
/* klass declaration                                              */
/*----------------------------------------------------------------*/

declare_klass(EVENTS_KLASS) {
    .size = KLASS_SIZE(events_t),
    .name = KLASS_NAME(events_t),
    .ctor = _events_ctor,
    .dtor = _events_dtor,
};

/*----------------------------------------------------------------*/
/* klass interface                                                */
/*----------------------------------------------------------------*/

events_t *events_create() {

    int stat = ERR;
    item_list_t items[1];
    events_t *self = NULL;

    SET_ITEM(items[0], 0, 0, 0, 0);

    self = (events_t *)object_create(EVENTS_KLASS, items, &stat);

    return self;

}

int events_destroy(events_t *self) {

    int stat = OK;

    when_error {

        if (self == NULL) {

            cause_error(E_INVPARM);
            
        }

        if (object_assert(self, events_t)) {

            stat = self->dtor(OBJECT(self));
            check_status(stat, OK, E_INVOPS);
            
        } else {

            cause_error(E_INVOBJ);

        }

        exit_when;

    } use {

        stat = ERR;

        object_set_error2(self, trace_errnum, trace_lineno, trace_filename, trace_function);
        clear_error();

    } end_when;

    return stat;

}

int events_override(events_t *self, item_list_t *items) {

    int stat = OK;

    when_error_in {
        
        if (self == NULL) {

            cause_error(E_INVPARM);
            
        }

        stat = self->_override(self, items);
        check_status(stat, OK, E_INVOPS);

        exit_when;

    } use {

        stat = ERR;

        object_set_error2(self, trace_errnum, trace_lineno, trace_filename, trace_function);
        clear_error();

    } end_when;

    return stat;

}

int events_compare(events_t *us, events_t *them) {

    int stat = OK;

    when_error_in {

        if (us == NULL) {

            cause_error(E_INVPARM);

        }

        if (object_assert(them, events_t)) {

            stat = us->_compare(us, them);
            check_status(stat, OK, E_NOTSAME);

        } else {

            cause_error(E_INVOBJ);

        }

        exit_when;

    } use {

        stat = ERR;

        object_set_error2(us, trace_errnum, trace_lineno, trace_filename, trace_function);
        clear_error();

    } end_when;

    return stat;

}

char *events_version(events_t *self) {

    char *version = VERSION;

    return version;

}

int events_loop(events_t *self) {

    int stat = OK;

    when_error_in {

        if (self == NULL) {

            cause_error(E_INVPARM);

        }

        stat = self->_loop(self);
        check_return(stat, self);

        exit_when;

    } use {

        stat = ERR;

        object_set_error2(self, trace_errnum, trace_lineno, trace_filename, trace_function);
        clear_error();

    } end_when;

    return stat;

}

int events_register_input(events_t *self, int fd, int (*input)(void *), void *data) {

    int stat = OK;

    when_error_in {

        if ((self == NULL) || (input == NULL)) {

            cause_error(E_INVPARM);

        }

        stat = self->_register_input(self, fd, input, data);
        check_return(stat, self);

        exit_when;

    } use {

        stat = ERR;

        object_set_error2(self, trace_errnum, trace_lineno, trace_filename, trace_function);
        clear_error();

    } end_when;

    return stat;

}

int events_register_worker(events_t *self, int reque, int (*input)(void *), void *data) {

    int stat = OK;

    when_error_in {

        if ((self == NULL) || (input == NULL)) {

            cause_error(E_INVPARM);

        }

        stat = self->_register_worker(self, reque, input, data);
        check_return(stat, self);

        exit_when;

    } use {

        stat = ERR;

        object_set_error2(self, trace_errnum, trace_lineno, trace_filename, trace_function);
        clear_error();

    } end_when;

    return stat;

}

int events_register_timer(events_t *self, int reque, double interval, int (*input)(void *), void *data) {

    int stat = OK;

    when_error_in {

        if ((self == NULL) || (input == NULL)) {

            cause_error(E_INVPARM);

        }

        stat = self->_register_timer(self, reque, interval, input, data);
        check_return(stat, self);

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

int _events_ctor(object_t *object, item_list_t *items) {

    int stat = OK;
    events_t *self = NULL;

    if (object != NULL) {

        /* capture our items */

        if (items != NULL) {

            int x;
            for (x = 0;; x++) {

                if ((items[x].buffer_length == 0) &&
                    (items[x].item_code == 0)) break;

                /* switch(items[x].item_code) { */
                /*     case EVENTS_K_TYPE: { */
                /*         memcpy(&type,  */
                /*                items[x].buffer_address,  */
                /*                items[x].buffer_length); */
                /*         break; */
                /*     } */
                /* } */

            }

        }

        /* initilize our base klass here */

        object_set_error1(object, OK);

        /* initialize our derived klass here */

        self = EVENTS(object);

        /* assign our methods here */

        self->ctor = _events_ctor;
        self->dtor = _events_dtor;
        self->_compare = _events_compare;
        self->_override = _events_override;

        self->_loop = _events_loop;
        self->_register_input = _events_register_input;
        self->_register_timer = _events_register_timer;
        self->_register_worker = _events_register_worker;

        /* initialize internal variables here */

        when_error_in {

            /* create a "self pipe" for signal handling */

            stat = _init_self_pipe(self);
            check_status(stat, OK, E_INVOPS);

            /* initialize the handlers queue */

            stat = que_init(&self->handlers);
            check_status(stat, QUE_OK, E_INVOPS);

            exit_when;

        } use {

            stat = ERR;

            object_set_error2(self, trace_errnum, trace_lineno, trace_filename, trace_function);
            clear_error();

        } end_when;

    }

    return stat;

}

int _events_dtor(object_t *object) {

    int stat = OK;
    events_t *self = EVENTS(object);
    events_handler_t *handler = NULL;

    /* free local resources here */

    NxRemoveInput(NULL, pipe_id);

    while ((handler = que_pop_head(&self->handlers))) {
        
        switch (handler->type) {
            case EV_INPUT:
                NxRemoveInput(NULL, handler->input_id);
                break;
            case EV_WORKER:
                NxRemoveWorkProc(NULL, handler->worker_id);
                break;
            case EV_TIMER:
                NxRemoveTimeOut(NULL, handler->timer_id);
                break;
        }
        
        free(handler);
        
    }
    
    /* walk the chain, freeing as we go */

    object_demote(object, object_t);
    object_destroy(object);

    return stat;

}

int _events_override(events_t *self, item_list_t *items) {

    int stat = ERR;

    if (items != NULL) {

        int x;
        for (x = 0;; x++) {

            if ((items[x].buffer_length == 0) &&
                (items[x].item_code == 0)) break;

            switch(items[x].item_code) {
                case EVENTS_M_DESTRUCTOR: {
                    self->dtor = items[x].buffer_address;
                    stat = 0;
                    break;
                }
            }

        }

    }

    return stat;

}

int _events_compare(events_t *self, events_t *other) {

    int stat = ERR;

    if ((object_compare(OBJECT(self), OBJECT(other)) == 0) &&
        (self->ctor == other->ctor) &&
        (self->dtor == other->dtor) &&
        (self->_compare == other->_compare) &&
        (self->_override == other->_override) &&
        (self->_loop == other->_loop) &&
        (self->_register_input == other->_register_input) &&
        (self->_register_worker == other->_register_worker) &&
        (self->_register_timer == other->_register_timer)) {

        stat = OK;

    }

    return stat;

}

int _events_register_input(events_t *self, int fd, int (*input)(void *), void *data) {

    int stat = OK;
    callback_t *callback = NULL;
    events_handler_t *handler= NULL;

    when_error_in {

        errno = 0;
        if ((callback = calloc(1, sizeof(callback_t))) == NULL) {
            
            cause_error(errno);
            
        }
        
        errno = 0;
        if ((handler = calloc(1, sizeof(events_handler_t))) == NULL) {
            
            cause_error(errno);
            
        }
        
        callback->data = data;
        callback->self = self;
        callback->input = input;

        handler->type = EV_INPUT;
        handler->input_id = NxAddInput(NULL, fd, NxInputReadMask, _dispatch_input, callback);
        
        stat = que_push_head(&self->handlers, handler);
        check_status(stat, QUE_OK, E_NOQUEUE);

        exit_when;

    } use {

        stat = ERR;

        object_set_error2(self, trace_errnum, trace_lineno, trace_filename, trace_function);
        clear_error();

    } end_when;

    return stat;

}

int _events_register_worker(events_t * self, int reque, int (*input)(void *), void *data) {

    int stat = OK;
    callback_t *callback = NULL;
    events_handler_t *handler= NULL;

    when_error_in {

        errno = 0;
        if ((callback = calloc(1, sizeof(callback_t))) == NULL) {
            
            cause_error(errno);
            
        }
        
        errno = 0;
        if ((handler = calloc(1, sizeof(events_handler_t))) == NULL) {
            
            cause_error(errno);
            
        }
        
        callback->data = data;
        callback->self = self;
        callback->input = input;
        callback->reque = reque;

        handler->type = EV_WORKER;
        handler->worker_id = NxAddWorkProc(NULL, _dispatch_worker, callback);

        stat = que_push_head(&self->handlers, handler);
        check_status(stat, QUE_OK, E_NOQUEUE);

        exit_when;

    } use {

        stat = ERR;

        object_set_error2(self, trace_errnum, trace_lineno, trace_filename, trace_function);
        clear_error();

    } end_when;

    return stat;

}

int _events_register_timer(events_t *self, int reque, double interval, int (*input)(void *), void *data) {

    int stat = OK;
    callback_t *callback = NULL;
    events_handler_t *handler= NULL;

    when_error_in {

        errno = 0;
        if ((callback = calloc(1, sizeof(callback_t))) == NULL) {

            cause_error(errno);

        }

        errno = 0;
        if ((handler = calloc(1, sizeof(events_handler_t))) == NULL) {

            cause_error(errno);

        }

        callback->data = data;
        callback->self = self;
        callback->input = input;
        callback->reque = reque;
        callback->interval = interval;

        handler->type = EV_TIMER;
        handler->timer_id = NxAddTimeOut(NULL, interval, _dispatch_timer, callback);

        stat = que_push_head(&self->handlers, handler);
        check_status(stat, QUE_OK, E_NOQUEUE);

        exit_when;

    } use {

        stat = ERR;

        object_set_error2(self, trace_errnum, trace_lineno, trace_filename, trace_function);
        clear_error();

    } end_when;

    return stat;

}

int _events_loop(events_t *self) {

    int stat = OK;

    pipe_id = NxAddInput(NULL, pfd[0], NxInputReadMask, _read_pipe, (void *)self);

    stat = NxMainLoop(NULL);

    return stat;

}

/*----------------------------------------------------------------*/
/* private methods                                                */
/*----------------------------------------------------------------*/

static int _dispatch_input(NxAppContext context, NxInputId id, int fd, void *data) {

    callback_t *callback = (callback_t *)data;

    (*callback->input)(callback->data);

    return OK;

}

static int _dispatch_worker(NxAppContext context, NxWorkProcId id, void *data) {

    NxWorkProcId temp_id;
    events_handler_t *temp = NULL;
    events_handler_t *handler = NULL;
    callback_t *callback = (callback_t *)data;
    events_t *self = (events_t *)callback->self;

    (*callback->input)(callback->data);

    for (handler = que_first(&self->handlers);
         handler != NULL;
         handler = que_next(&self->handlers)) {

        if (handler->type == EV_WORKER) {

            if (handler->worker_id == id) {

                temp = que_delete(&self->handlers);

                if (callback->reque) {

                    temp_id = NxAddWorkProc(NULL, _dispatch_worker, callback);
                    temp->worker_id = temp_id;

                    que_push_tail(&self->handlers, temp);

                } else {

                    free(temp);

                }

            }

        }

    }
        
    return OK;

}

static int _dispatch_timer(NxAppContext context, NxIntervalId id, void *data) {

    NxIntervalId temp_id;
    events_handler_t *temp = NULL;
    events_handler_t *handler = NULL;
    callback_t *callback = (callback_t *)data;
    events_t *self = (events_t *)callback->self;

    (*callback->input)(callback->data);

    for (handler = que_first(&self->handlers);
         handler != NULL;
         handler = que_next(&self->handlers)) {

        if (handler->type == EV_TIMER) {

            if (handler->timer_id == id) {

                temp = que_delete(&self->handlers);

                if (callback->reque) {

                    temp_id = NxAddTimeOut(NULL, callback->interval, _dispatch_timer, callback);
                    temp->timer_id = temp_id;

                    que_push_tail(&self->handlers, temp);

                } else {

                    free(temp);

                }

            }

        }

    }

    return OK;

}

static int _read_pipe(NxAppContext context, NxInputId id, int source, void *data) {

    int sig;
    int size = 0;
    int stat = OK;
    events_t *self = (events_t *)data;

    when_error_in {

        for (;;) {                      /* Consume bytes from pipe */

            errno = 0;
            if ((size = read(pfd[0], &sig, sizeof(int)) > 0)) {

                if ((sig == SIGINT) || (sig == SIGTERM)) {

                    /* preform cleanup */

                    stat = self->dtor((object_t *)self);

                    /* reinstall signal handlers */

                    errno = 0;
                    if (sigaction(SIGINT, &old_sigint, NULL) != 0) {

                        cause_error(errno);

                    }

                    errno = 0;
                    if (sigaction(SIGTERM, &old_sigterm, NULL) != 0) {

                        cause_error(errno);

                    }

                    /* re-raise the signal, normal signal handling  */
                    /* should now do it's own cleanup and cleanly   */
                    /* exits.                                       */

                    raise(sig);

                }

            } else {

                cause_error(errno);

            }

        }

        exit_when;

    } use {

        stat = ERR;

        object_set_error2(self, trace_errnum, trace_lineno, trace_filename, trace_function);
        clear_error();

    } end_when;

    return stat;

}

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

static int _init_self_pipe(events_t *self) {

    int flags = 0;
    int stat = OK;
    struct sigaction sa;

    when_error_in {
        
        /* create our pipe */

        errno = 0;
        if (pipe(pfd) == -1) {

            cause_error(errno);

        }

        /* Make read end nonblocking */

        errno = 0;
        if ((flags = fcntl(pfd[0], F_GETFL)) == -1) {

            cause_error(errno);

        }

        errno = 0;
        flags |= O_NONBLOCK;                
        if (fcntl(pfd[0], F_SETFL, flags) == -1) {

            cause_error(errno);

        }

        /* Make write end nonblocking */

        errno = 0;
        if ((flags = fcntl(pfd[1], F_GETFL)) == -1) {

            cause_error(errno);

        }

        errno = 0;
        flags |= O_NONBLOCK;                
        if (fcntl(pfd[1], F_SETFL, flags) == -1) {

            cause_error(errno);

        }

        /* we would like to capture SIGINT and SIGTERM to      */
        /* perform resource cleanup. just using our own        */
        /* signal handlers lead to memory corruption and       */
        /* dumping core, so this work around was implemented.  */
        /*                                                     */
        /* this is a shim to capture signal handlers so that   */
        /* we can hook in our own signal handlers to clean up  */
        /* resources. _events_read_pipe() reinstalls the saved */
        /* signal handlers to allow normal signal handling.    */

        /* capture signal handlers */

        errno = 0;
        if ((sigaction(SIGTERM, NULL, &old_sigterm)) != 0) {

            cause_error(errno);

        }

        errno = 0;
        if ((sigaction(SIGINT, NULL, &old_sigint)) != 0) {

            cause_error(errno);

        }

        /* set up our signal handlers. */

        sigemptyset(&sa.sa_mask);
        sa.sa_flags |= SA_RESTART;
        sa.sa_handler = _sig_handler;

        errno = 0;
        if (sigaction(SIGINT, &sa, NULL) == -1) {

            cause_error(errno);
            
        }

        errno = 0;
        if (sigaction(SIGTERM, &sa, NULL) == -1) {

            cause_error(errno);

        }

        exit_when;

    } use {
        
        stat = ERR;

        object_set_error2(self, trace_errnum, trace_lineno, trace_filename, trace_function);
        clear_error();
        
    } end_when;
    
    return stat;

}

