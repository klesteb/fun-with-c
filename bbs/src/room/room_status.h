
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

#define RS_FORGET  (1L<<1)   /* forget this room                          */
#define RS_REMOVED (1L<<2)   /* removed from this room                    */
#define RS_INVITED (1L<<3)   /* invited into room                         */

/*-------------------------------------------------------------*/
/* data structures                                             */
/*-------------------------------------------------------------*/

typedef struct _room_status_s {
    long roomnum;           /* room number                               */
    long usernum;           /* user number                               */
    ushort flags;           /* room status                               */
    int revision;           /* the revision of this record               */
} room_status_t;

typedef struct _room_status_search_s {
    long roomnum;           /* room number                               */
    long usernum;           /* user number                               */
    off_t record;           /* record number                             */
} room_status_search_t;

typedef struct _room_status_find_s {
    long roomnum;           /* room number                               */
    long usernum;           /* user number                               */
} room_status_find_t;

/*-------------------------------------------------------------*/
/* klass interface                                             */
/*-------------------------------------------------------------*/

extern rms_t *room_status_create(char *, int, int, int, tracer_t *);
extern int room_status_capture(rms_t *, void *, queue *);
extern char *room_status_version(rms_t *);

#define room_status_destroy(self) rms_destroy(self)
#define room_status_compare(self, other) rms_compare(self, other)
#define room_status_override(self, items) rms_override(self, items)

#define room_status_open(self) rms_open(self)
#define room_status_close(self) rms_close(self);
#define room_status_del(self, recnum) rms_del(self, recnum)
#define room_status_extend(self, amount) rms_extend(self, amount)
#define room_status_add(self, user) rms_add(self, (void *)user)
#define room_status_get(self, recnum, user) rms_get(self, recnum, (void *)user)
#define room_status_put(self, recnum, user) rms_put(self, recnum, (void *)user)
#define room_status_find(self, data, len, compare, recnum) rms_find(self, data, len, compare, recnum)
#define room_status_search(self, data, len, compare, capture, results) rms_search(self, data, len, compare, capture, results)

#endif

