
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

#include "files.h"
#include "tracer.h"
#include "object.h"
#include "datatypes.h"
#include "item_list.h"

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
    char username[LEN_NAME+1];      /* Name for Citadel messages & mail */
    char password[LEN_PASSWD+1];    /* password (for BBS-only users)    */
    char axlevel;                   /* Access level                     */
    unsigned qwk;                   /* See QWK_ flags above             */
    unsigned flags;                 /* See US_ flags above              */
    int screenwidth;                /* For formatting messages          */
    int screenlength;               /* lines before MORE                */
    int timescalled;                /* Number of logins                 */
    int posted;                     /* Number of messages posted (ever) */
    long eternal;                   /* Eternal user number              */
    long today;                     /* time spent online today          */
    long timelimit;                 /* user's time limit in minutes     */
    long online;                    /* Total Number of seconds spent onl */
    off_t profile;                  /* pointer to profile record        */
    time_t lastcall;                /* Last time the user called        */
    time_t firstcall;               /* time of first call               */
} user_base_t;

typedef struct _user_profile_t {
    off_t userdb;                   /* pointer to user record           */
    char name[LEN_NAME+1];          /* user's real full name            */
    char addr[LEN_ADDRESS+1];       /* user's address                   */
    char city[LEN_CITY+1];          /* user's address                   */
    char state[LEN_STATE+1];        /* user's address                   */
    char zip[LEN_ZIP+1];            /* user's address                   */
    char phone[LEN_PHONE+1];        /* user's phone number              */
    char email[LEN_EMAIL+1];        /* user's email address             */
    char description[LEN_DESC+1];   /* user description                 */
} user_profile_t;

/*-------------------------------------------------------------*/
/* klass defination                                            */
/*-------------------------------------------------------------*/

typedef struct _user_s user_t;

struct _user_s {
    object_t parent_klass;
    int (*ctor)(object_t *, item_list_t *);
    int (*dtor)(object_t *);
    int (*_compare)(user_t *, user_t *);
    int (*_override)(user_t *, item_list_t *);

    int (*_open)(user_t *);
    int (*_close)(user_t *);
    int (*_unlock)(user_t *);
    int (*_del)(user_t *, long);
    int (*_extend)(user_t *, int);
    int (*_lock)(user_t *, off_t);
    int (*_add)(user_t *, user_base_t *);
    int (*_get_sequence)(user_t *, long *);
    int (*_get)(user_t *, int, user_base_t *);
    int (*_put)(user_t *, int, user_base_t *);
    int (*_next)(user_t *, user_base_t *, ssize_t *);
    int (*_prev)(user_t *, user_base_t *, ssize_t *);
    int (*_last)(user_t *, user_base_t *, ssize_t *);
    int (*_read)(user_t *, user_base_t *, ssize_t *);
    int (*_write)(user_t *, user_base_t *, ssize_t *);
    int (*_first)(user_t *, user_base_t *, ssize_t *);
    int (*_build)(user_t *, user_base_t *, user_base_t *);

    int index;
    int users;
    int locked;
    int retries;
    int timeout;
    char *path;
    files_t *userdb;
    files_t *sequence;
    files_t *profiles;
    tracer_t *trace;
};

/*-------------------------------------------------------------*/
/* klass constants                                             */
/*-------------------------------------------------------------*/

#define USER(x) ((user_t *)(x))

#define USER_K_PATH    1
#define USER_K_RETRIES 2
#define USER_K_TIMEOUT 3
#define USER_K_TRACE   4
#define USER_K_USERS   5

#define USER_M_DESTRUCTOR 1
#define USER_M_OPEN       2
#define USER_M_CLOSE      3
#define USER_M_UNLOCK     4
#define USER_M_LOCK       5
#define USER_M_GET        6
#define USER_M_PUT        7
#define USER_M_NEXT       8
#define USER_M_PREV       9
#define USER_M_LAST       10
#define USER_M_WRITE      11
#define USER_M_FIRST      12
#define USER_M_BUILD      13
#define USER_M_EXTEND     14
#define USER_M_ADD        15
#define USER_M_DEL        16

/*-------------------------------------------------------------*/
/* klass interface                                             */
/*-------------------------------------------------------------*/

extern user_t *user_create(char *, int, int, int, tracer_t *);
extern int user_destroy(user_t *);
extern int user_compare(user_t *, user_t *);
extern int user_override(user_t *, item_list_t *);
extern char *user_version(user_t *);

extern int user_open(user_t *);
extern int user_close(user_t *);
extern int user_del(user_t *, ushort);
extern int user_extend(user_t *, int);
extern int user_index(user_t *, int *);
extern int user_add(user_t *, user_base_t *);
extern int user_get(user_t *, int, user_base_t *);
extern int user_put(user_t *, int, user_base_t *);
extern int user_next(user_t *, user_base_t *, ssize_t *);
extern int user_prev(user_t *, user_base_t *, ssize_t *);
extern int user_last(user_t *, user_base_t *, ssize_t *);
extern int user_first(user_t *, user_base_t *, ssize_t *);

#endif

