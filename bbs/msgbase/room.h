
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

#include "jam.h"
#include "files.h"
#include "object.h"
#include "tracer.h"
#include "que_util.h"
#include "datatypes.h"
#include "item_list.h"
#include "error_code.h"

/*-------------------------------------------------------------*/
/* data structures                                             */
/*-------------------------------------------------------------*/

typedef struct _room_base_s {
    char name[32];          /* name of the room                          */
    char path[256];         /* path to the message base                  */
    short conference;       /* the qwk conference number                 */
    short flags;            /* capability flags                          */
    int retries;            /* number of retires for file locking        */
    int timeout;            /* timeout in seconds, between retries       */
    int base;               /* the base message number                   */
} room_base_t;              /* 304 bytes                                 */

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
    int (*_del)(room_t *, short);
    int (*_add)(room_t *, room_base_t *);
    int (*_get)(room_t *, short, room_base_t *);
    int (*_put)(room_t *, short, room_base_t *);
    int (*_next)(room_t *, room_base_t *, ssize_t *);
    int (*_prev)(room_t *, room_base_t *, ssize_t *);
    int (*_last)(room_t *, room_base_t *, ssize_t *);
    int (*_read)(room_t *, room_base_t *, ssize_t *);
    int (*_first)(room_t *, room_base_t *, ssize_t *);
    int (*_write)(room_t *, room_base_t *, ssize_t *);
    int (*_build)(room_t *, room_base_t *, room_base_t *);

    int base;
    int retries;
    int timeout;
    char *msgbase;
    jam_t *jam;
    files_t *rooms;
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

#define ROOM_M_DESTRUCTOR 1

/*-------------------------------------------------------------*/
/* constants                                                   */
/*-------------------------------------------------------------*/

#define INUSE       1   /* flags mask               */
#define PUBLIC      2   /* flags mask               */
#define FILEDIR     4   /* flags mask               */
#define PERMROOM    8   /* flags mask               */

/*-------------------------------------------------------------*/
/* klass interface                                             */
/*-------------------------------------------------------------*/

extern room_t *room_create(char *, char *, int, int, tracer_t *);
extern int room_destroy(room_t *);
extern int room_compare(room_t *, room_t *);
extern int room_override(room_t *, item_list_t *);
extern char *room_version(room_t *);

extern int room_open(room_t *);
extern int room_close(room_t *);
extern int room_add(room_t *, room_base_t *);
extern int room_get(room_t *, short, room_base_t *);
extern int room_put(room_t *, short, room_base_t *);
extern int room_next(room_t *, room_base_t *, ssize_t *);
extern int room_prev(room_t *, room_base_t *, ssize_t *);
extern int room_last(room_t *, room_base_t *, ssize_t *);
extern int room_first(room_t *, room_base_t *, ssize_t *);

#endif

