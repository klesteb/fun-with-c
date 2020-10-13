
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
#include "../tracer/tracer.h"

/**
 * @file when.h
 * @author Kevin L. Esteb (kevin@kesteb.us)
 * @date 15-Nov-2019
 * 
 * @brief A macro package to implement structured error handling.
 * 
 * @par Description
 * This is a macro package to implement a structured error handling
 * solution. This takes a different path from the more common 
 * "try/catch" approach. This emulates the DEC/HP Basic "when error"
 * blocks for error handling. 
 * 
 * There are some caveats; you can't nest blocks, you can't easily 
 * propagate an error up the call chain, there is no retry logic. 
 * 
 * But it does handle the chore of checking return codes and then
 * branching to an error handler when an error occurs. Errors are 
 * determined if an OK or ERR is returned.
 * 
 **/

#define when_error { static error_trace_t _er_trace;
#define when_error_in { static error_trace_t _er_trace;
#define use handler:
#define end_when } exit_handler: 
#define exit_when goto exit_handler

#define trace_errnum   _er_trace.errnum
#define trace_lineno   _er_trace.lineno
#define trace_filename _er_trace.filename
#define trace_function _er_trace.function

#define cause_error(error) {            \
    trace_errnum = (error);             \
    trace_lineno = __LINE__;            \
    trace_filename = strdup(__FILE__);  \
    trace_function = strdup(__func__);  \
    goto handler;                       \
}

#define clear_error(error) {            \
    trace_errnum = 0;                   \
    trace_lineno = 0;                   \
    free(trace_filename);               \
    free(trace_function);               \
}

#define copy_error(error) {                        \
    (*(error)).errnum = trace_errnum;              \
    (*(error)).lineno = trace_lineno;              \
    (*(error)).filename = strdup(trace_filename);  \
    (*(error)).function = strdup(trace_function);  \
}

#define clear_copied(error) {                    \
    (error).errnum = 0;                          \
    (error).lineno = 0;                          \
    free((error).filename);                      \
    free((error).function);                      \
}

#define capture_error(error) {                   \
    trace_errnum = (error).errnum;               \
    trace_lineno = (error).lineno;               \
    free(trace_filename);                        \
    trace_filename = strdup((error).filename);   \
    free(trace_function);                        \
    trace_function = strdup((error).function);   \
}

#define retrieve_error(self) {                   \
    object_get_error(OBJECT(self), &_er_trace);  \
}

#define check_status(status, expected, error) {  \
    if ((status) != (expected)) {                \
        trace_errnum = (error);                  \
        trace_lineno = __LINE__ - 1;             \
        trace_filename = strdup(__FILE__);       \
        trace_function = strdup(__func__);       \
        goto handler;                            \
    }                                            \
}

#define check_status2(status, expected, error) { \
    if ((status) != (expected)) {                \
        capture_error((error));                  \
        clear_copied((error));                   \
        goto handler;                            \
    }                                            \
}

#define check_return(status, self) {             \
    if ((status) != (OK)) {                      \
        retrieve_error((self));                  \
        goto handler;                            \
    }                                            \
}

#define check_creation(self) {                   \
    retrieve_error((self));                      \
    if (trace_errnum != (OK)) {                  \
        goto handler;                            \
    }                                            \
}

#endif

