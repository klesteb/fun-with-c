
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

#ifndef _LOGGER_H
#define _LOGGER_H

#include <stdarg.h>
#include <log4c.h>

#include "objects/object.h"

/*----------------------------------------------------------------*/
/* klass declaration                                              */
/*----------------------------------------------------------------*/

typedef struct _logger_s logger_t;

struct _logger_s {
    object_t parent_klass;
    int   (*ctor)(object_t *, item_list_t *);
    int   (*dtor)(object_t *);
    int   (*_dispatch)(logger_t *, int, int, char *, const char *, char *);
    int  pid;
    char *hostname;
    char *category;
    char *process;
    char *facility;
};

/*-------------------------------------------------------------*/
/* constants                                                   */
/*-------------------------------------------------------------*/

#define LOGGER(x) ((logger_t *)(x))

#define INFO  1
#define WARN  2
#define ERROR 3
#define FATAL 4
#define DEBUG 5
#define TRACE 6

#define LOGGER_K_FACILITY 1
#define LOGGER_K_CATEGORY 2
#define LOGGER_K_PROCESS  3

#define log_info(self, fmt, args...) \
    log_dispatch((self), INFO, __LINE__, __FILE__, __FUNCTION__, (fmt), ##args)

#define log_warn(self, fmt, args...) \
    log_dispatch((self), WARN, __LINE__, __FILE__, __FUNCTION__, (fmt), ##args)

#define log_error(self, fmt, args...) \
    log_dispatch((self), ERROR, __LINE__, __FILE__, __FUNCTION__, (fmt), ##args)

#define log_fatal(self, fmt, args...) \
    log_dispatch((self), FATAL, __LINE__, __FILE__, __FUNCTION__, (fmt), ##args)

#define log_debug(self, fmt, args...) \
    log_dispatch((self), DEBUG, __LINE__, __FILE__, __FUNCTION__, (fmt), ##args)

#define log_trace(self, fmt, args...) \
    log_dispatch((self), TRACE, __LINE__, __FILE__, __FUNCTION__, (fmt), ##args)

/*-------------------------------------------------------------*/
/* interface                                                   */
/*-------------------------------------------------------------*/

extern logger_t *log_create(char *, char *, char *);
extern int log_destroy(logger_t *);
extern int log_set_category(logger_t *, char *);
extern int log_get_category(logger_t *, char *, int);
extern int log_set_facility(logger_t *, char *);
extern int log_get_facility(logger_t *, char *, int);
extern int log_set_process(logger_t *, char *);
extern int log_get_process(logger_t *, char *, int);
extern int log_dispatch(logger_t *, int, int, char *, const char *, char *, ...);

#endif

