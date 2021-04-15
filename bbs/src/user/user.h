
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

#ifndef _USER_H
#define _USER_H

#include "rms.h"
#include "tracer.h"

/*-------------------------------------------------------------*/
/* constants                                                   */
/*-------------------------------------------------------------*/

/* field lengths */

#define LEN_NAME     31
#define LEN_PASSWD   15
#define LEN_ADDRESS  31
#define LEN_CITY     31
#define LEN_STATE    31
#define LEN_ZIP      31
#define LEN_PHONE    31
#define LEN_EMAIL    47
#define LEN_DESC     1023

/* user.axlevel values */

#define AX_DEL       0
#define AX_NEW       1
#define AX_TWIT      2
#define AX_NORM      3
#define AX_NET       4
#define AX_PREF      5
#define AX_AIDE      6
#define AX_SYSOP     7

/* bits in user.flags */

#define US_SYSOP     (1L<<0)   /* Sysop access                     */            
#define US_ROOMAIDE  (1L<<1)   /* Is the user granted privs in rm  */
#define US_NEEDVALID (1L<<2)   /* User needs to be validated       */
#define US_SUBSYSTEM (1L<<3)   /* User has subsystem access        */
#define US_PERM      (1L<<4)   /* Permanent user                   */
#define US_EXPERT    (1L<<5)   /* Experienced user                 */
#define US_NOHIDE    (1L<<6)   /* Hide reginfo from regular users  */
#define US_REGIS     (1L<<7)   /* Registered user                  */
#define US_PROFILE   (1L<<8)   /* Has the user entered a profile?  */
#define US_INACTIVE  (1L<<9)   /* Inactive account                 */
#define US_DELETED   (1L<<10)  /* User has been deleted            */

/* bits in user.qwk */

#define QWK_FILES   (1L<<0)    /* Include new files list            */
#define QWK_EMAIL   (1L<<1)    /* Include unread e-mail             */
#define QWK_ALLMAIL (1L<<2)    /* Include ALL e-mail                */
#define QWK_DELMAIL (1L<<3)    /* Delete e-mail after download      */
#define QWK_BYSELF  (1L<<4)    /* Include messages from self        */
#define QWK_UNUSED  (1L<<5)    /* Currently unused                  */
#define QWK_EXPCTLA (1L<<6)    /* Expand ctrl-a codes to ascii      */
#define QWK_RETCTLA (1L<<7)    /* Retain ctrl-a codes               */
#define QWK_ATTACH  (1L<<8)    /* Include file attachments          */
#define QWK_NOINDEX (1L<<9)    /* Do not create index files in QWK  */
#define QWK_TZ      (1L<<10)   /* Include "@TZ" time zone in msgs   */
#define QWK_VIA     (1L<<11)   /* Include "@VIA" seen-bys in msgs   */
#define QWK_NOCTRL  (1L<<12)   /* No extraneous control files       */

/*-------------------------------------------------------------*/
/* data structures                                             */
/*-------------------------------------------------------------*/

/* users information */

typedef struct _user_base_s {         
    char username[LEN_NAME+1];      /* Name for messages & mail         */
    char axlevel;                   /* See AX_ flags above              */
    unsigned qwk;                   /* See QWK_ flags above             */
    unsigned flags;                 /* See US_ flags above              */
    int screenwidth;                /* For formatting messages          */
    int screenlength;               /* Lines before MORE                */
    int timescalled;                /* Number of logins                 */
    int posted;                     /* Number of messages posted (ever) */
    long eternal;                   /* Eternal user number              */
    long today;                     /* Time spent online today          */
    long timelimit;                 /* User's time limit in minutes     */
    long online;                    /* Total Number of seconds spent onl */
    off_t profile;                  /* Pointer to profile record        */
    time_t lastcall;                /* Last time the user called        */
    time_t firstcall;               /* Time of first call               */
    int revision;                   /* Record revision                  */
} user_base_t;

typedef struct _user_profile_s {
    char name[LEN_NAME+1];          /* user's real full name            */
    char addr[LEN_ADDRESS+1];       /* user's address                   */
    char city[LEN_CITY+1];          /* user's address                   */
    char state[LEN_STATE+1];        /* user's address                   */
    char zip[LEN_ZIP+1];            /* user's address                   */
    char phone[LEN_PHONE+1];        /* user's phone number              */
    char email[LEN_EMAIL+1];        /* user's email address             */
    char description[LEN_DESC+1];   /* user description                 */
    int revision;                   /* record revision                  */
} user_profile_t;

typedef struct _user_search_s {
    char username[LEN_NAME+1];      /* Name for messages & mail         */
    off_t record;                   /* index of user record             */
    off_t profile;                  /* pointer to the user profile      */
} user_search_t;

/*-------------------------------------------------------------*/
/* klass interface                                             */
/*-------------------------------------------------------------*/

extern rms_t *user_create(char *, int, int, int, tracer_t *);
extern int user_capture(rms_t *, void *, queue *);
extern char *user_version(rms_t *);

#define user_destroy(self) rms_destroy(self)
#define user_compare(self, other) rms_compare(self, other)
#define user_override(self, items) rms_override(self, items)

#define user_open(self) rms_open(self)
#define user_close(self) rms_close(self);
#define user_del(self, recnum) rms_del(self, recnum)
#define user_extend(self, amount) rms_extend(self, amount)
#define user_add(self, user) rms_add(self, (void *)user)
#define user_get(self, recnum, user) rms_get(self, recnum, (void *)user)
#define user_put(self, recnum, user) rms_put(self, recnum, (void *)user)
#define user_find(self, data, len, compare, recnum) rms_find(self, data, len, compare, recnum)
#define user_search(self, data, len, compare, capture, results) rms_search(self, data, len, compare, capture, results)

#endif

