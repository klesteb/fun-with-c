
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
#include "item_list.h"

#if !defined(linux)
#if !(((defined(__FreeBSD__) && __FreeBSD_version >= 440000)) || defined(NeXTBSD))
typedef unsigned short ushort;   /* must be 16 bits wide */
#endif
typedef unsigned long  ulong;    /* must be 32 bits wide */
#endif
typedef unsigned char  uchar;    /* must be  8 bits wide */
typedef unsigned int   uint;

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
    long index;             /* Converted MSBIN floating format           */
    char conference;        /* Conference number - should be ignored     */
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

    files_t *messages;
    files_t *control;
    files_t *ndx;
    tracer_t *trace;
};

/*-------------------------------------------------------------*/
/* constants                                                   */
/*-------------------------------------------------------------*/

#define QWK(x) ((qwk_t *)(x))

#define QWK_M_DESTRUCTOR 1


#define QWK_BLOCK_SIZE    128
#define QWK_RECORD(n)     ((((n) - QWK_BLOCK_SIZE) / QWK_BLOCK_SIZE) + 1)

#define QWK_PUB_UNREAD    ' '
#define QWK_PUB_READ      '-'
#define QWK_PRIVATE       '*'
#define QWK_SYS_UNREAD    '~'
#define QWK_SYS_READ      '`'
#define QWK_PROT_UNREAD   '%'
#define QWK_PROT_READ     '^'
#define QWK_GRP_UNREAD    '!'
#define QWK_GRP_READ      '#'
#define QWK_PROT_ALL      '$'
#define QWK_NET_TAG       '*'

/*-------------------------------------------------------------*/
/* interface                                                   */
/*-------------------------------------------------------------*/

extern qwk_t *qwk_create(item_list_t *);
extern int qwk_destroy(qwk_t *);
extern int qwk_compare(qwk_t *, qwk_t *);
extern int qwk_override(qwk_t *, item_list_t *);
extern char *qwk_version(qwk_t *);

#endif

