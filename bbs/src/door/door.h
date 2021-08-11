
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

/*-------------------------------------------------------------*/
/* constants                                                   */
/*-------------------------------------------------------------*/

#define DOOR_NAME_LEN 31
#define DOOR_DESC_LEN 63
#define DOOR_CMD_LEN  255

/* bits in flags */

#define DF_DELETED   (1L<<1)   /* door has been deleted             */
#define DF_DOORINFO  (1L<<2)   /* create a doorinfo1.def drop file  */
#define DF_DOORSYS   (1L<<3)   /* create a door.sys drop file       */
#define DF_DOOR32    (1L<<4)   /* create a door32.sys drop files    */

/*-------------------------------------------------------------*/
/* data structures                                             */
/*-------------------------------------------------------------*/

typedef struct _door_base_s {         
    char name[DOOR_NAME_LEN+1];         /* Name of door                 */
    char description[DOOR_DESC_LEN+1];  /* description of the door      */
    char command[DOOR_CMD_LEN+1];       /* command to run door          */
    ulong flags;                        /* See DF_ flags above          */
    int revision;                       /* Record revision              */
} door_base_t;

typedef struct _door_search_s {
    char name[DOOR_NAME_LEN+1];         /* Name of door                 */
    off_t record;                       /* index of door record         */
} door_search_t;

/*-------------------------------------------------------------*/
/* klass interface                                             */
/*-------------------------------------------------------------*/

extern rms_t *door_create(char *, int, int, int, tracer_t *);
extern int door_capture(rms_t *, void *, queue *);
extern char *door_version(rms_t *);

#define door_destroy(self) rms_destroy(self)
#define door_compare(self, other) rms_compare(self, other)
#define door_override(self, items) rms_override(self, items)

#define door_open(self) rms_open(self)
#define door_close(self) rms_close(self);
#define door_del(self, recnum) rms_del(self, recnum)
#define door_extend(self, amount) rms_extend(self, amount)
#define door_add(self, door) rms_add(self, (void *)door)
#define door_get(self, recnum, door) rms_get(self, recnum, (void *)door)
#define door_put(self, recnum, door) rms_put(self, recnum, (void *)door)
#define door_find(self, data, len, compare, recnum) rms_find(self, data, len, compare, recnum)
#define door_search(self, data, len, compare, capture, results) rms_search(self, data, len, compare, capture, results)

#endif

