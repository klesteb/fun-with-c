
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

#ifndef _RMS_H
#define _RMS_H

#include "rms/files.h"
#include "tracer/tracer.h"
#include "objects/object.h"
#include "cclibs/que_util.h"
#include "include/item_list.h"

/*----------------------------------------------------------------*/
/* klass macros                                                   */
/*----------------------------------------------------------------*/

#define RMS_OFFSET(n, s)   ((((n) - 1) * (s)))
#define RMS_RECORD(n, s)   (((n) / (s)) + 1)

/*-------------------------------------------------------------*/
/* klass defination                                            */
/*-------------------------------------------------------------*/

typedef struct _rms_s rms_t;

struct _rms_s {
    object_t parent_klass;
    int (*ctor)(object_t *, item_list_t *);
    int (*dtor)(object_t *);
    int (*_compare)(rms_t *, rms_t *);
    int (*_override)(rms_t *, item_list_t *);

    int (*_init)(rms_t *);
    int (*_open)(rms_t *);
    int (*_close)(rms_t *);
    int (*_remove)(rms_t *);
    int (*_unlock)(rms_t *);
    int (*_del)(rms_t *, off_t);
    int (*_add)(rms_t *, void *);
    int (*_extend)(rms_t *, int);
    int (*_lock)(rms_t *, off_t);
    int (*_tell)(rms_t *, off_t *);
    int (*_record)(rms_t *, off_t *);
    int (*_seek)(rms_t *, off_t, int);
    int (*_get)(rms_t *, off_t, void *);
    int (*_put)(rms_t *, off_t, void *);
    int (*_get_sequence)(rms_t *, long *);
    int (*_peek_sequence)(rms_t *, long *);
    int (*_build)(rms_t *, void *, void *);
    int (*_read)(rms_t *, void *, ssize_t *);
    int (*_next)(rms_t *, void *, ssize_t *);
    int (*_prev)(rms_t *, void *, ssize_t *);
    int (*_last)(rms_t *, void *, ssize_t *);
    int (*_first)(rms_t *, void *, ssize_t *);
    int (*_write)(rms_t *, void *, ssize_t *);
    int (*_normalize)(rms_t *, void *, void *);
    int (*_find)(rms_t *, void *, int, int (*compare)(void *, void *), off_t *);
    int (*_search)(rms_t *, void *, int, int (*compare)(void *, void *), int (*capture)(rms_t *, void *, queue *), queue *);

    int size;
    int record;
    int records;
    int recsize;
    int locked;
    int retries;
    int timeout;
    char *path;
    files_t *rmsdb;
    files_t *sequence;
    tracer_t *trace;
};

/*-------------------------------------------------------------*/
/* klass constants                                             */
/*-------------------------------------------------------------*/

#define RMS(x) ((rms_t *)(x))

#define RMS_K_PATH    1
#define RMS_K_RETRIES 2
#define RMS_K_TIMEOUT 3
#define RMS_K_TRACE   4
#define RMS_K_RECORDS 5
#define RMS_K_RECSIZE 6
#define RMS_K_NAME    7

#define RMS_M_DESTRUCTOR 1
#define RMS_M_OPEN       2
#define RMS_M_CLOSE      3
#define RMS_M_UNLOCK     4
#define RMS_M_LOCK       5
#define RMS_M_GET        6
#define RMS_M_PUT        7
#define RMS_M_NEXT       8
#define RMS_M_PREV       9
#define RMS_M_LAST       10
#define RMS_M_WRITE      11
#define RMS_M_FIRST      12
#define RMS_M_BUILD      13
#define RMS_M_EXTEND     14
#define RMS_M_ADD        15
#define RMS_M_DEL        16
#define RMS_M_NORMALIZE  17
#define RMS_M_FIND       18
#define RMS_M_SEARCH     19
#define RMS_M_RECORD     20
#define RMS_M_INIT       21
#define RMS_M_SEEK       22
#define RMS_M_TELL       23

/*-------------------------------------------------------------*/
/* klass interface                                             */
/*-------------------------------------------------------------*/

extern rms_t *rms_create(char *, char *, int, int, int, int, tracer_t *);
extern int rms_destroy(rms_t *);
extern int rms_compare(rms_t *, rms_t *);
extern int rms_override(rms_t *, item_list_t *);
extern char *rms_version(rms_t *);

extern int rms_open(rms_t *);
extern int rms_close(rms_t *);
extern int rms_remove(rms_t *);
extern int rms_del(rms_t *, off_t);
extern int rms_add(rms_t *, void *);
extern int rms_extend(rms_t *, int);
extern int rms_tell(rms_t *, off_t *);
extern int rms_record(rms_t *, off_t *);
extern int rms_seek(rms_t *, off_t, int);
extern int rms_get(rms_t *, off_t, void *);
extern int rms_put(rms_t *, off_t, void *);
extern int rms_find(rms_t *, void *, int,  int (*compare)(void *, void *), off_t *);
extern int rms_search(rms_t *, void *, int,  int (*compare)(void *, void *), int (*capture)(rms_t *, void *, queue *), queue *);

#endif

