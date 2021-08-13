
/*---------------------------------------------------------------------------*/
/*                Copyright (c) 2019 by Kevin L. Esteb                       */
/*                                                                           */
/*  Permission to use, copy, modify, and distribute this software and its    */
/*  documentation for any purpose and without fee is hereby granted,         */
/*  provided that this copyright notice appears in all copies. The author    */
/*  makes no representations about the suitability of this software for      */
/*  any purpose. It is provided "as is" without express or implied warranty. */
/*                                                                           */
/*---------------------------------------------------------------------------*/

#ifndef _DOOR_H
#define _DOOR_H

#include "files/files.h"
#include "tracer/tracer.h"
#include "objects/object.h"
#include "include/item_list.h"

#include "bbs/src/room/room.h"
#include "bbs/src/node/node.h"
#include "bbs/src/user/user.h"
#include "bbs/src/user/profile.h"

/*-------------------------------------------------------------*/
/* klass defination                                            */
/*-------------------------------------------------------------*/

typedef struct _door_s door_t;

struct _door_s {
    object_t parent_klass;
    int (*ctor)(object_t *, item_list_t *);
    int (*dtor)(object_t *);
    int (*_compare)(door_t *, door_t *);
    int (*_override)(door_t *, item_list_t *);
    int (*_open)(door_t *);
    int (*_close)(door_t *);
    int (*_remove)(door_t *);
    int (*_run)(door_t *, node_base_t *, room_base_t *, user_base_t *, user_base_t *, profile_base_t *);

    ulong flags;
    int retries;
    int timeout;
    char *path;
    char *command;
    files_t *control;
    tracer_t *trace;
};

/*-------------------------------------------------------------*/
/* klass constants                                             */
/*-------------------------------------------------------------*/

#define DF_DOOR32   (1L<<1)
#define DF_DOORSYS  (1L<<2)
#define DF_DOORINFO (1L<<3)

#define DOOR_K_PATH    1
#define DOOR_K_TRACE   2
#define DOOR_K_RETRIES 3
#define DOOR_K_TIMEOUT 4

#define DOOR(x) ((door_t *)(x))

#define DOOR_M_DESTRUCTOR 1

/*-------------------------------------------------------------*/
/* klass interface                                             */
/*-------------------------------------------------------------*/

extern door_t *door_create(char *, int, int, tracer_t *);
extern int door_destroy(door_t *);
extern int door_compare(door_t *, door_t *);
extern int door_override(door_t *, item_list_t *);
extern char *door_version(door_t *);

extern int door_open(door_t *);
extern int door_close(door_t *);
extern int door_remove(door_t *);
extern int door_run(door_t *, node_base_t *, room_base_t *, user_base_t *, user_base_t *, profile_base_t *);

#endif

