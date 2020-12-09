
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

#ifndef _QWK_H
#define _QWK_H

#include <time.h>
#include <errno.h>

#include "files.h"
#include "tracer.h"
#include "object.h"
#include "que_util.h"
#include "fnm_util.h"
#include "item_list.h"
#include "datatypes.h"

/*-------------------------------------------------------------*/
/* data structures                                             */
/*-------------------------------------------------------------*/

typedef struct _header_s {
    char  status;           /* message status byte                       */
    long  number;           /* .QWK = message number, .REP = conference  */
    time_t date_time;       /* Date and time in Unix format              */
    char  to[26];           /* Person addressed to                       */
    char  from[26];         /* Person who wrote message                  */
    char  subject[26];      /* Subject of message                        */
    char  password[13];     /* password in ascii                         */
    long  reply;            /* message # this refers to                  */
    long  records;          /* # of 128 byte recs in message, inc header */
    char  alive;            /* always 0xE1                               */
    short conference;       /* area number                               */
    short seq_number;       /* logical message number                    */
    char  net_tag;          /* space: no tag, "*": tag present ???       */
} qwk_header_t;

typedef struct _ndx_s {     /* <area>.ndx                                */
    ulong index;            /* Converted MSBIN floating format           */
    uchar conference;       /* Conference number - should be ignored     */
} qwk_ndx_t;

typedef struct _controls_s {
    char bbs_name[32];      /* The bbs's name                            */
    char city[32];          /* The City BBS resides in                   */
    char state[32];         /* State BBS resides in                      */
    char phone[16];         /* BBS's phone number                        */
    char sysop[32];         /* The Sysop's name                          */
    char serial_num[32];    /* The Mail Door serial number               */
    char bbs_id[32];        /* The name of the BBS ID                    */
    time_t date_time;       /* Date and time of packet in Unix format    */
    char username[32];      /* The Users name                            */
    long num_areas;         /* Number of Areas                           */
    char hello_file[32];    /* Name of hello file                        */
    char news_file[32];     /* Name of news file                         */
    char goodbye_file[32];  /* Name of goodbye file                      */
    queue areas;            /* The areas                                 */
} qwk_control_t;

typedef struct _area_s {
    long area;              /* Area number                               */
    char name[32];          /* Area Name (truncated to 10 chars)         */
} qwk_area_t;

/*-------------------------------------------------------------*/
/* klass defination                                            */
/*-------------------------------------------------------------*/

typedef struct _qwk_s qwk_t;

struct _qwk_s {
    object_t parent_klass;
    int (*ctor)(object_t *, item_list_t *);
    int (*dtor)(object_t *);
    int (*_compare)(qwk_t *, qwk_t *);
    int (*_override)(qwk_t *, item_list_t *);
    int (*_open)(qwk_t *);
    int (*_close)(qwk_t *);
    int (*_close_ndx)(qwk_t *);
    int (*_open_ndx)(qwk_t *, char *);
    int (*_free_text)(qwk_t *, char *);
    int (*_put_notice)(qwk_t *, char *);
    int (*_get_notice)(qwk_t *, char **); 
    int (*_new_header)(qwk_t *, qwk_header_t **);
    int (*_get_control)(qwk_t *, qwk_control_t *); 
    int (*_put_control)(qwk_t *, qwk_control_t *);
    int (*_get_ndx)(qwk_t *, qwk_ndx_t *, ssize_t *);
    int (*_put_ndx)(qwk_t *, qwk_ndx_t *, ssize_t *);
    int (*_new_ndx)(qwk_t *, ulong, uchar, qwk_ndx_t **);
    int (*_get_message)(qwk_t *, ulong, qwk_header_t *, char **);
    int (*_put_message)(qwk_t *, qwk_header_t *, char *, ulong *);

    int rep;
    int retries;
    int timeout;
    char *path;
    files_t *messages;
    files_t *control;
    files_t *ndx;
    tracer_t *trace;
};

/*-------------------------------------------------------------*/
/* klass constants                                             */
/*-------------------------------------------------------------*/

#define QWK(x) ((qwk_t *)(x))

#define QWK_K_PATH    1
#define QWK_K_REP     2
#define QWK_K_TRACE   3
#define QWK_K_RETRIES 4
#define QWK_K_TIMEOUT 5

#define QWK_M_DESTRUCTOR  1
#define QWK_M_FREE_TEXT   2
#define QWK_M_PUT_NOTICE  3
#define QWK_M_GET_NOTICE  4
#define QWK_M_GET_CONTROL 5
#define QWK_M_PUT_CONTROL 6
#define QWK_M_GET_NDX     7
#define QWK_M_PUT_NDX     8
#define QWK_M_GET_MESSAGE 9
#define QWK_M_PUT_MESSAGE 10
#define QWK_M_OPEN_NDX    11
#define QWK_M_CLOSE_NDX   12
#define QWK_M_OPEN        13
#define QWK_M_CLOSE       14
#define QWK_M_NEW_HEADER  15
#define QWK_M_NEW_NDX     16

/*-------------------------------------------------------------*/
/* constants                                                   */
/*-------------------------------------------------------------*/

#define QWK_BLOCK_SIZE  128

#define QWK_PUB_UNREAD  ' '
#define QWK_PUB_READ    '-'
#define QWK_PRIVATE     '*'
#define QWK_SYS_UNREAD  '~'
#define QWK_SYS_READ    '`'
#define QWK_PROT_UNREAD '%'
#define QWK_PROT_READ   '^'
#define QWK_GRP_UNREAD  '!'
#define QWK_GRP_READ    '#'
#define QWK_PROT_ALL    '$'
#define QWK_NET_TAG     '*'

/*----------------------------------------------------------------*/
/* macros                                                         */
/*----------------------------------------------------------------*/

#define QWK_BUF_SIZE(n) (((QWK_BLOCK_SIZE * (n))))
#define QWK_ISALIVE(n)  ((n) == 0xe1 ? TRUE : FALSE)
#define QWK_OFFSET(n)   ((((n) - 1) * QWK_BLOCK_SIZE))
#define QWK_REC_CNT(n)  ((((n) / QWK_BLOCK_SIZE) + (((n) % QWK_BLOCK_SIZE) != 0)))
#define QWK_RECORD(n)   (((n) / QWK_BLOCK_SIZE) + 1)
/*#define QWK_RECORD(n)   ((((n) - QWK_BLOCK_SIZE) / QWK_BLOCK_SIZE) + 1)*/

/*-------------------------------------------------------------*/
/* interface                                                   */
/*-------------------------------------------------------------*/

extern qwk_t *qwk_create(char *, int, int, int, tracer_t *);
extern int qwk_destroy(qwk_t *);
extern int qwk_compare(qwk_t *, qwk_t *);
extern int qwk_override(qwk_t *, item_list_t *);
extern char *qwk_version(qwk_t *);

extern int qwk_open(qwk_t *);
extern int qwk_close(qwk_t *);
extern int qwk_close_ndx(qwk_t *);
extern int qwk_open_ndx(qwk_t *, char *);
extern int qwk_free_text(qwk_t *, char *);
extern int qwk_put_notice(qwk_t *, char *); 
extern int qwk_get_notice(qwk_t *, char **); 
extern int qwk_get_control(qwk_t *, qwk_control_t *); 
extern int qwk_put_control(qwk_t *, qwk_control_t *);
extern int qwk_new_header( qwk_t *, qwk_header_t **);
extern int qwk_get_ndx(qwk_t *, qwk_ndx_t *, ssize_t *);
extern int qwk_put_ndx(qwk_t *, qwk_ndx_t *, ssize_t *);
extern int qwk_new_ndx(qwk_t *, ulong, uchar, qwk_ndx_t **);
extern int qwk_get_message(qwk_t *, ulong, qwk_header_t *, char **);
extern int qwk_put_message(qwk_t *, qwk_header_t *, char *, ulong *);

#endif

