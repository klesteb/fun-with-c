
/*---------------------------------------------------------------------------*/
/*                Copyright (c) 2020 by Kevin L. Esteb                       */
/*                                                                           */
/*  Permission to use, copy, modify, and distribute this software and its    */
/*  documentation for any purpose and without fee is hereby granted,         */
/*  provided that the above copyright notice appears in all copies. The      */
/*  author makes no representations about the suitability of this software   */
/*  for any purpose. It is provided "as is" without express or implied       */
/*  warranty.                                                                */
/*---------------------------------------------------------------------------*/

#ifndef _TRACE_H
#define _TRACE_H

#include "when.h"
#include "object.h"
#include "errors.h"
#include "que_util.h"
#include "item_list.h"
#include "error_trace.h"

/*-------------------------------------------------------------*/
/* klass defination                                            */
/*-------------------------------------------------------------*/

typedef struct _tracer_s tracer_t;

struct _tracer_s {
    object_t parent_klass;
    int (*ctor)(object_t *, item_list_t *);
    int (*dtor)(object_t *);
    int (*_compare)(tracer_t *, tracer_t *);
    int (*_override)(tracer_t *, item_list_t *);
    int (*_add)(tracer_t *, error_trace_t *);
    int (*_dump)(tracer_t *, int (*output)(char *));
    errors_t *errs;
    queue errors;
};

/*-------------------------------------------------------------*/
/* constants                                                   */
/*-------------------------------------------------------------*/

#define TRACER(x) ((tracer_t *)(x))

#define TRACER_K_ERRORS 1

#define TRACER_M_DESTRUCTOR 1
#define TRACER_M_ADD        2
#define TRACER_M_DUMP       3

/*----------------------------------------------------------------*/
/* macros                                                         */
/*----------------------------------------------------------------*/

#undef check_return

#define check_return(status, self) {       \
    if ((status) != (OK)) {                \
        retrieve_error((self));            \
        trace_lineno = __LINE__ - 1;       \
        free(trace_filename);              \
        trace_filename = strdup(__FILE__); \
        free(trace_function);              \
        trace_function = strdup(__func__); \
        goto handler;                      \
    }                                      \
}

#undef check_status2
    
#define check_status2(status, expected, error) { \
    if ((status) != (expected)) {                \
        capture_error((error));                  \
        trace_lineno = __LINE__ - 1;             \
        free(trace_filename);                    \
        trace_filename = strdup(__FILE__);       \
        free(trace_function);                    \
        trace_function = strdup(__func__);       \
        clear_copied((error));                   \
        goto handler;                            \
    }                                            \
}
    
#define capture_trace(dump) {                                \
    if ((dump) != NULL) {                                    \
        error_trace_t *junk = malloc(sizeof(error_trace_t)); \
        if (junk != NULL) {                                  \
            junk->errnum = trace_errnum;                     \
            junk->lineno = trace_lineno;                     \
            junk->filename = strdup(trace_filename);         \
            junk->function = strdup(trace_function);         \
            tracer_add((dump), junk);                        \
        }                                                    \
    }                                                        \
}

#define process_error(self) {     \
    capture_trace((self)->trace); \
    object_set_error2(self, trace_errnum, trace_lineno, trace_filename, trace_function); \
    clear_error();                \
}

/*-------------------------------------------------------------*/
/* interface                                                   */
/*-------------------------------------------------------------*/

extern tracer_t *tracer_create(errors_t *);
extern int tracer_destroy(tracer_t *);
extern int tracer_compare(tracer_t *, tracer_t *);
extern int tracer_override(tracer_t *, item_list_t *);
extern int tracer_add(tracer_t *, error_trace_t *);
extern int tracer_dump(tracer_t *, int (*output)(char *));
extern char *tracer_version(tracer_t *);

#endif

