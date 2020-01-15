
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

#include <unistd.h>
#include <stdarg.h>
#include <stdio.h>
#include <errno.h>

#include "when.h"
#include "object.h"
#include "logger.h"
#include "logger_priv.h"
#include "error_codes.h"
#include "log4c_extensions.h"

require_klass(OBJECT_KLASS);

/*----------------------------------------------------------------*/
/* klass defination                                               */
/*----------------------------------------------------------------*/

int _log_ctor(object_t *object, item_list_t *);
int _log_dtor(object_t *);

int _log_dispatch(logger_t *, int, int, char *, const char *, char *);

declare_klass(LOGGER) {
    .size = KLASS_SIZE(logger_t),
    .name = KLASS_NAME(logger_t),
    .ctor = _log_ctor,
    .dtor = _log_dtor,
};

/*----------------------------------------------------------------*/
/* klass interface                                                */
/*----------------------------------------------------------------*/

logger_t *log_create(char *facility, char *category, char *process) {

    int stat = ERR;
    item_list_t items[4];
    logger_t *self = NULL;

    SET_ITEM(items[0], LOGGER_K_FACILITY, facility, strlen(facility), NULL);
    SET_ITEM(items[1], LOGGER_K_CATEGORY, category, strlen(category), NULL);
    SET_ITEM(items[2], LOGGER_K_PROCESS, process, strlen(process), NULL);
    SET_ITEM(items[3], 0, 0, 0, 0);

    self = (logger_t *)object_create(LOGGER, items, &stat);
    
    return self;

}

int log_destroy(logger_t *self) {

    int stat = OK;

    when_error {

        if (self != NULL) {

            if (object_assert(self, logger_t)) {

                stat = self->dtor((object_t *)self);
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

int log_set_category(logger_t *self, char *category) {

    int stat = OK;

    when_error {

        if ((self != NULL) && (category != NULL)) {

            free(self->category);
            self->category = strdup(category);

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

int log_get_category(logger_t *self, char *category, int size) {

    int stat = OK;

    when_error {

        if ((self != NULL) && (category != NULL) && (size > 0)) {

            strncpy(category, self->category, size);
            category[size] = '\0';

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

int log_set_facility(logger_t *self, char *facility) {

    int stat = OK;

    when_error {

        if ((self != NULL) && (facility != NULL)) {

            free(self->facility);
            self->facility = strdup(facility);

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

int log_get_facility(logger_t *self, char *facility, int size) {

    int stat = ERR;

    when_error {

        if ((self != NULL) && (facility != NULL) && (size > 0)) {

            strncpy(facility, self->facility, size);
            facility[size] = '\0';

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

int log_set_process(logger_t *self, char *process) {

    int stat = OK;

    when_error {

        if ((self != NULL) && (process != NULL)) {

            free(self->process);
            self->process = strdup(process);

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

int log_get_process(logger_t *self, char *process, int size) {

    int stat = OK;

    when_error {

        if ((self != NULL) && (process != NULL) && (size > 0)) {

            strncpy(process, self->process, size);
            process[size] = '\0';

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

int log_dispatch(logger_t *self, int type, int lineno, char *filename, const char *function, char *fmt, ...) {

    va_list aptr;
    int stat = OK;
    char buffer[2048];

    when_error {

        if (self != NULL) {

            va_start(aptr, fmt);
            vsnprintf(buffer, 2047, fmt, aptr);
            va_end(aptr);

            stat = self->_dispatch(self, type, lineno, filename, function, buffer);
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

int _log_ctor(object_t *object, item_list_t *items) {

    int rc = 0;
    int stat = ERR;
    char *hostname = NULL;
    char *process = NULL;
    char *category = NULL;
    char *facility = NULL;
    logger_t *self = NULL;

    if (object != NULL) {

        /* capture our items */

        if (items != NULL) {

            int x;
            for (x = 0;; x++) {

                if ((items[x].buffer_length == 0) &&
                    (items[x].item_code == 0)) break;

                switch(items[x].item_code) {
                    case LOGGER_K_FACILITY: {
                        facility = strndup(items[x].buffer_address,
                                           items[x].buffer_length);
                        break;
                    }
                    case LOGGER_K_CATEGORY: {
                        category = strndup(items[x].buffer_address,
                                           items[x].buffer_length);
                        break;
                    }
                    case LOGGER_K_PROCESS: {
                        process = strndup(items[x].buffer_address,
                                          items[x].buffer_length);
                        break;
                    }

                }

            }

        }

        /* initilize our base klass here */

        object_set_error(object, OK);

        /* initialize our derived klass here */

        self = LOGGER(object);

        /* assign our methods here */

        self->ctor = _log_ctor;
        self->dtor = _log_dtor;

        self->_dispatch = _log_dispatch;

        /* initialize internal variables here */

        self->hostname = NULL;
        self->facility = NULL;
        self->category = NULL;
        self->process  = NULL;

        if (log4c_extensions_init()) {

            stat = ERR;
            object_set_error(self, E_NOLOAD);
            goto fini;

        }

        if (log4c_init()) {

            stat = ERR;
            object_set_error(self, E_NOLOAD);
            goto fini;

        }

        errno = 0;
        if ((hostname = calloc(1, 1024)) == NULL) {

            stat = ERR;
            object_set_error(self, errno);
            goto fini;

        }

        errno = 0;
        if ((rc = gethostname(hostname, 1023)) == -1) {

            stat = ERR;
            object_set_error(self, errno);
            goto fini;

        }

        self->pid = getpid();
        self->process = process;
        self->facility = facility;
        self->category = category;
        self->hostname = hostname;

        stat = OK;

    }

    fini:
    return stat;

}

int _log_dtor(object_t *object) {

    int stat = OK;

    /* free local resources here */

    logger_t *self = LOGGER(object);

    free(self->hostname);
    free(self->category);
    free(self->facility);
    free(self->process);

    log4c_fini();

    /* walk the chain, freeing as we go */

    object_demote(object, object_t);
    object_destroy(object);

    return stat;

}

int _log_dispatch(logger_t *self, int type, int lineno, char *filename, const char *function, char *message) {

    int stat = ERR;
    user_local_info_t user;
    log4c_location_info_t locinfo;
    log4c_category_t *mycat = log4c_category_get(self->category);

    user.pid = self->pid;
    user.process = self->process;
    user.facility = self->facility;
    user.hostname = self->hostname;
    user.category = self->category;
    
    locinfo.loc_data = &user;
    locinfo.loc_line = lineno;
    locinfo.loc_file = filename;
    locinfo.loc_function = function;

    switch (type) {
        case INFO: {
            stat = OK;
            log4c_category_log_locinfo(mycat, &locinfo, LOG4C_PRIORITY_INFO, message);
            break;
        }
        case WARN: {
            stat = OK;
            log4c_category_log_locinfo(mycat, &locinfo, LOG4C_PRIORITY_WARN, message);
            break;
        }
        case ERROR: {
            stat = OK;
            log4c_category_log_locinfo(mycat, &locinfo, LOG4C_PRIORITY_ERROR, message);
            break;
        }
        case FATAL: {
            stat = OK;
            log4c_category_log_locinfo(mycat, &locinfo, LOG4C_PRIORITY_FATAL, message);
            break;
        }
        case DEBUG: {
            stat = OK;
            log4c_category_log_locinfo(mycat, &locinfo, LOG4C_PRIORITY_DEBUG, message);
            break;
        }
        case TRACE: {
            stat = OK;
            log4c_category_log_locinfo(mycat, &locinfo, LOG4C_PRIORITY_TRACE, message);
            break;
        }
    }

    return stat;

}

