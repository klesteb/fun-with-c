
/*---------------------------------------------------------------------------*/
/*                Copyright (c) 2020 by Kevin L. Esteb                       */
/*                                                                           */
/*  Permission to use, copy, modify, and distribute this software and its    */
/*  documentation for any purpose and without fee is hereby granted,         */
/*  provided that this copyright notice appears in all copies. The author    */
/*  makes no representations about the suitability of this software for      */
/*  any purpose. It is provided "as is" without express or implied warranty. */
/*                                                                           */
/*---------------------------------------------------------------------------*/

#ifndef _ROOM_H
#define _ROOM_H

#include "files.h"
#include "object.h"
#include "tracer.h"
#include "que_util.h"
#include "datatypes.h"
#include "item_list.h"
#include "error_code.h"
#include "jam/jam.h"

/*-------------------------------------------------------------*/
/* constants                                                   */
/*-------------------------------------------------------------*/

#define MAILROOM 1          /* mail is always room 0           */
#define LOBBY    2          /* lobby is always room 1          */
#define AIDEROOM 3          /* aide is always room 2           */

#define INUSE    (1L<<0)    /* room is in use                  */
#define PUBLIC   (1L<<1)    /* room is public                  */
#define ISDIR    (1L<<2)    /* room is a directory             */
#define PERMROOM (1L<<3)    /* room is permament               */
#define QWKNET   (1L<<4)    /* room is netted via qwk          */

/*-------------------------------------------------------------*/
/* data structures                                             */
/*-------------------------------------------------------------*/

typedef struct _room_base_s {
    long roomnum;           /* room number                               */
    char name[32];          /* name of the room                          */
    char path[256];         /* path to the message base                  */
    short conference;       /* the qwk conference number                 */
    short flags;            /* capability flags                          */
    int retries;            /* number of retires for file locking        */
    int timeout;            /* timeout in seconds, between retries       */
    int base;               /* the base message number                   */
    int revision;           /* the revision of this record               */
} room_base_t;

typedef struct _room_search_s {
    char name[32];          /* name of the room                          */
    short conference;       /* the qwk conference number                 */
} room_search_t;

/*-------------------------------------------------------------*/
/* klass defination                                            */
/*-------------------------------------------------------------*/

typedef struct _room_s room_t;

struct _room_s {
    object_t parent_klass;
    int (*ctor)(object_t *, item_list_t *);
    int (*dtor)(object_t *);
    int (*_compare)(room_t *, room_t *);
    int (*_override)(room_t *, item_list_t *);

    int (*_open)(room_t *);
    int (*_close)(room_t *);
    int (*_unlock)(room_t *);
    int (*_del)(room_t *, int);
    int (*_extend)(room_t *, int);
    int (*_lock)(room_t *, off_t);
    int (*_size)(room_t *, ssize_t *);
    int (*_add)(room_t *, room_base_t *);
    int (*_get_sequence)(room_t *, long *);
    int (*_get)(room_t *, int, room_base_t *);
    int (*_put)(room_t *, int, room_base_t *);
    int (*_next)(room_t *, room_base_t *, ssize_t *);
    int (*_prev)(room_t *, room_base_t *, ssize_t *);
    int (*_last)(room_t *, room_base_t *, ssize_t *);
    int (*_read)(room_t *, room_base_t *, ssize_t *);
    int (*_first)(room_t *, room_base_t *, ssize_t *);
    int (*_write)(room_t *, room_base_t *, ssize_t *);
    int (*_build)(room_t *, room_base_t *, room_base_t *);
    int (*_normalize)(room_t *, room_base_t *, room_base_t *);

    int base;
    int index;
    int rooms;
    int locked;
    int retries;
    int timeout;
    char *msgbase;
    jam_t *jam;
    files_t *roomdb;
    files_t *sequence;
    tracer_t *trace;
};

/*-------------------------------------------------------------*/
/* klass constants                                             */
/*-------------------------------------------------------------*/

#define ROOM(x) ((room_t *)(x))

#define ROOM_K_MSGBASE  1
#define ROOM_K_DATABASE 2
#define ROOM_K_RETRIES  3
#define ROOM_K_TIMEOUT  4
#define ROOM_K_TRACE    5
#define ROOM_K_BASE     6
#define ROOM_K_ROOMS    7

#define ROOM_M_DESTRUCTOR 1
#define ROOM_M_OPEN       2
#define ROOM_M_CLOSE      3
#define ROOM_M_DEL        4
#define ROOM_M_SIZE       5
#define ROOM_M_ADD        6
#define ROOM_M_GET        7
#define ROOM_M_PUT        8
#define ROOM_M_READ       9
#define ROOM_M_NEXT       10
#define ROOM_M_PREV       11
#define ROOM_M_LAST       12
#define ROOM_M_FIRST      13
#define ROOM_M_WRITE      14
#define ROOM_M_BUILD      15
#define ROOM_M_LOCK       16
#define ROOM_M_UNLOCK     17
#define ROOM_M_EXTEND     18
#define ROOM_M_NORMALIZE  19

/*-------------------------------------------------------------*/
/* klass interface                                             */
/*-------------------------------------------------------------*/

extern room_t *room_create(char *, char *, int, int, int, int, tracer_t *);
extern int room_destroy(room_t *);
extern int room_compare(room_t *, room_t *);
extern int room_override(room_t *, item_list_t *);
extern char *room_version(room_t *);

extern int room_open(room_t *);
extern int room_close(room_t *);
extern int room_del(room_t *, int);
extern int room_extend(room_t *, int);
extern int room_index(room_t *, int *);
extern int room_size(room_t *, ssize_t *);
extern int room_add(room_t *, room_base_t *);
extern int room_get(room_t *, int, room_base_t *);
extern int room_put(room_t *, int, room_base_t *);
extern int room_next(room_t *, room_base_t *, ssize_t *);
extern int room_prev(room_t *, room_base_t *, ssize_t *);
extern int room_last(room_t *, room_base_t *, ssize_t *);
extern int room_first(room_t *, room_base_t *, ssize_t *);

#endif

