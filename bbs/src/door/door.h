
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

#ifndef _DOOR_H
#define _DOOR_H

#include "rms/rms.h"
#include "tracer/tracer.h"
#include "bbs/src/main/bbs_config.h"

/*-------------------------------------------------------------*/
/* constants                                                   */
/*-------------------------------------------------------------*/

#define DF_DOOR32    (1L<<1)        /* uses a door32.sys drop file      */
#define DF_DOORSYS   (1L<<2)        /* uses a door.sys drop file        */
#define DF_DOORINFO  (1L<<3)        /* uses a doorinfo1.def drop file   */

#define DT_MULTIUSER (1L<<1)        /* allow multi-user access          */
#define DT_PATHDIR   (1L<<2)        /* create drop file in door path    */
#define DT_FREETIME  (1L<<3)        /* free time while in this door     */

#define DOOR_CMD_LEN  256
#define DOOR_PATH_LEN 256

/*-------------------------------------------------------------*/
/* data structures                                             */
/*-------------------------------------------------------------*/

typedef struct _door_usage_s {
    ulong runs;                     /* number of times ran by user      */
    ulong wasted;                   /* time wasted (in minutes) by user */
} door_usage_t;

typedef struct _door_base_s {
    char command[DOOR_CMD_LEN+1];   /* command to run door              */
    char clean[DOOR_CMD_LEN+1];     /* command to clear cruff           */
    char path[DOOR_PATH_LEN+1];     /* path to start door in            */
    door_usage_t usage[USERNUM];    /* door usage stats                 */
    ulong type;                     /* see DT_ types above              */
    ulong flags;                    /* see DF_ flags above              */
    ulong cost;                     /* cost of using door               */
    int active;                     /* wither the door is active        */
    int revision;                   /* record revision                  */
} door_base_t;

/*-------------------------------------------------------------*/
/* klass interface                                             */
/*-------------------------------------------------------------*/

extern rms_t *door_create(char *, char *, int, int, tracer_t *);
extern char *door_version(rms_t *);

#define door_destroy(self) rms_destroy(self)
#define door_compare(self, other) rms_compare(self, other)
#define door_override(self, items) rms_override(self, items)

#define door_open(self) rms_open(self)
#define door_close(self) rms_close(self);
#define door_remove(self) rms_remove(self);
#define door_get(self, data) rms_get(self, 1, (void *)data)
#define door_put(self, data) rms_put(self, 1, (void *)data)

#endif

