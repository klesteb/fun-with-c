
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

#ifndef _ROOM_STATUS_H
#define _ROOM_STATUS_H

#include "rms.h"
#include "tracer.h"

/*-------------------------------------------------------------*/
/* constants                                                   */
/*-------------------------------------------------------------*/

/*-------------------------------------------------------------*/
/* data structures                                             */
/*-------------------------------------------------------------*/

typedef struct _room_status_s {
    long roomnum;           /* room number                               */
    long usernum;           /* user number                               */
    ushort flags;           /* room status                               */
    int revision;           /* the revision of this record               */
} room_status_t;

typedef struct _status_search_s {
    long roomnum;           /* room number                               */
    long usernum;           /* user number                               */
    off_t record;           /* record number                             */
} status_search_t;

typedef struct _status_find_s {
    long roomnum;           /* room number                               */
    long usernum;           /* user number                               */
} status_find_t;

/*-------------------------------------------------------------*/
/* klass interface                                             */
/*-------------------------------------------------------------*/

extern rms_t *status_create(char *, int, int, int, tracer_t *);
extern int status_capture(rms_t *, void *, queue *);
extern char *status_version(rms_t *);

#define status_destroy(self) rms_destroy(self)
#define status_compare(self, other) rms_compare(self, other)
#define status_override(self, items) rms_override(self, items)

#define status_open(self) rms_open(self)
#define status_close(self) rms_close(self);
#define status_del(self, recnum) rms_del(self, recnum)
#define status_extend(self, amount) rms_extend(self, amount)
#define status_add(self, user) rms_add(self, (void *)user)
#define status_get(self, recnum, user) rms_get(self, recnum, (void *)user)
#define status_put(self, recnum, user) rms_put(self, recnum, (void *)user)
#define status_find(self, data, len, compare, recnum) rms_find(self, data, len, compare, recnum)
#define status_search(self, data, len, compare, capture, results) rms_search(self, data, len, compare, capture, results)

#endif

