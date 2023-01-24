
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

#ifndef _PROFILE_H
#define _PROFILE_H

#include "rms/rms.h"
#include "tracer/tracer.h"

#include "bbs/src/user/user_common.h"

/*-------------------------------------------------------------*/
/* constants                                                   */
/*-------------------------------------------------------------*/

/* bits in profile.flags */

#define PF_DELETED   (1L<<1)  /* Record has been deleted            */

/*-------------------------------------------------------------*/
/* data structures                                             */
/*-------------------------------------------------------------*/

typedef struct _profile_base_s {
    ulong flags;                    /* See PF_ flags above              */
    char name[LEN_NAME+1];          /* user's real full name            */
    char addr1[LEN_ADDRESS+1];      /* user's address - line 1          */
    char addr2[LEN_ADDRESS+1];      /* user's address - line 2          */
    char addr3[LEN_ADDRESS+1];      /* user's address - line 3          */
    char city[LEN_CITY+1];          /* user's city                      */
    char state[LEN_STATE+1];        /* user's state                     */
    char zip[LEN_ZIP+1];            /* user's zip                       */
    char phone[LEN_PHONE+1];        /* user's phone number              */
    char email[LEN_EMAIL+1];        /* user's email address             */
    char description[LEN_DESC+1];   /* user description                 */
    int revision;                   /* record revision                  */
} profile_base_t;

typedef struct _profile_search_s {
    char name[LEN_NAME+1];          /* profile name                     */
    off_t record;                   /* index of profile record          */
} profile_search_t;

/*-------------------------------------------------------------*/
/* klass interface                                             */
/*-------------------------------------------------------------*/

extern rms_t *profile_create(char *, int, int, int, tracer_t *);
extern int profile_capture(rms_t *, void *, queue *);
extern char *profile_version(rms_t *);

#define profile_destroy(self) rms_destroy(self)
#define profile_compare(self, other) rms_compare(self, other)
#define profile_override(self, items) rms_override(self, items)

#define profile_open(self) rms_open(self)
#define profile_close(self) rms_close(self);
#define profile_del(self, recnum) rms_del(self, recnum)
#define profile_extend(self, amount) rms_extend(self, amount)
#define profile_add(self, profile) rms_add(self, (void *)profile)
#define profile_get(self, recnum, profile) rms_get(self, recnum, (void *)profile)
#define profile_put(self, recnum, profile) rms_put(self, recnum, (void *)profile)
#define profile_find(self, data, len, compare, recnum) rms_find(self, data, len, compare, recnum)
#define profile_search(self, data, len, compare, capture, results) rms_search(self, data, len, compare, capture, results)

#endif

