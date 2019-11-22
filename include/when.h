
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

#ifndef _WHEN_H_
#define _WHEN_H_

#include <string.h>
#include <stdlib.h>

#include "object.h"
#include "error_trace.h"

#define when_error { static error_trace_t _er_trace;
#define error_handler handler:
#define end_when } exit_handler: 
#define exit_when goto exit_handler

#define trace_errnum   _er_trace.errnum
#define trace_lineno   _er_trace.lineno
#define trace_filename _er_trace.filename
#define trace_function _er_trace.function

#define cause_error(error) {                \
    _er_trace.errnum = (error);             \
    _er_trace.lineno = __LINE__;            \
    _er_trace.filename = strdup(__FILE__);  \
    _er_trace.function = strdup(__func__);  \
    goto handler;                           \
}

#define clear_error() {                     \
    _er_trace.errnum = 0;                   \
    _er_trace.lineno = 0;                   \
    free(_er_trace.filename);               \
    free(_er_trace.function);               \
}

#define check_status(status, error) {           \
    if ((status) != OK) {                       \
        _er_trace.errnum = (error);             \
        _er_trace.lineno = __LINE__ - 1;        \
        _er_trace.filename = strdup(__FILE__);  \
        _er_trace.function = strdup(__func__);  \
        goto handler;                           \
    }                                           \
}

#define check_return(status, self) {            \
    if ((status) != OK) {                       \
        object_get_error((self), &_er_trace);   \
        goto handler;                           \
    }                                           \
}

#define check_creation(self) {                      \
    if ((self) == NULL) {                           \
        object_get_error(OBJECT(self), &_er_trace); \
        goto handler;                               \
    }                                               \
}

#define retrieve_error(self) {                  \
    object_get_error(OBJECT(self), &_er_trace); \
    goto handler;                               \
}

#endif

