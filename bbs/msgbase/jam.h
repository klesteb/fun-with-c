
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

#ifndef _JAM_H
#define _JAM_H

#include "files.h"
#include "tracer.h"
#include "object.h"
#include "item_list.h"
#include "error_code.h"

#if !defined(linux)
#if !(((defined(__FreeBSD__) && __FreeBSD_version >= 440000)) || defined(NeXTBSD))
typedef unsigned short ushort;   /* must be 16 bits wide */
#endif
typedef unsigned long  ulong;    /* must be 32 bits wide */
#endif
typedef unsigned char  uchar;    /* must be  8 bits wide */

/*-------------------------------------------------------------*/
/* data structures                                             */
/*-------------------------------------------------------------*/

/* header file information block, stored first in all .JHR files */

typedef struct _base_header_s {
    uchar  signature[4];        /* <J><A><M> followed by <NUL>         */
    ulong  date_created;        /* Creation date                       */
    ulong  mod_counter;         /* Last processed counter              */
    ulong  active_msgs;         /* Number of active (not deleted) msgs */
    ulong  password_crc;        /* CRC-32 of password to access        */
    ulong  base_msg_num;        /* Lowest message number in index file */
    uchar  rsrvd[1000];         /* Reserved space                      */
} jam_header_t;

/* message header, one for each message */

typedef struct _meassage_header_s {
    uchar  signature[4];        /* <J><A><M> followed by <NUL>         */
    ushort revision;            /* CURRENTREVLEV                       */
    ushort reserved_word;       /* Reserved                            */
    ulong  subfield_length;     /* Length of Subfields                 */
    ulong  times_read;          /* Number of times message read        */
    ulong  msg_id_crc;          /* CRC-32 of MSGID line                */
    ulong  reply_crc;           /* CRC-32 of REPLY line                */
    ulong  reply_to;            /* This msg is a reply to..            */
    ulong  reply_1st;           /* First reply to this msg             */
    ulong  reply_next;          /* Next msg in reply chain             */
    ulong  date_written;        /* When msg was written                */
    ulong  date_received;       /* When msg was received/read          */
    ulong  date_processed;      /* When msg was processed by packer    */
    ulong  msgnum;              /* Message number (1-based)            */
    ulong  attribute;           /* Msg attribute, see "Status bits"    */
    ulong  attribute2;          /* Reserved for future use             */
    ulong  text_offset;         /* Offset of text in text file         */
    ulong  text_length;         /* Length of message text              */
    ulong  password_crc;        /* CRC-32 of password to access msg    */
    ulong  cost;                /* Cost of message                     */
} jam_message_t;

/* message header subfield */

typedef struct _field_s {
    ushort lo_id;               /* Field ID, 0 - 0xffff                */
    ushort hi_id;               /* Reserved for future use             */
    ulong  data_length;         /* Length of buffer that follows       */
    void  *buffer;              /* DatLen bytes of data                */
} jam_field_t;

typedef struct _save_field_s {
    ushort lo_id;               /* Field ID, 0 - 0xffff                */
    ushort hi_id;               /* Reserved for future use             */
    ulong  data_length;         /* Length of buffer that follows       */
} jam_save_field_t;

/*  message index record */

typedef struct _index_s {
    ulong  user_crc;            /* CRC-32 of destination username      */
    ulong  header_offset;       /* Offset of header in .JHR file       */
} jam_index_t;

/*  lastread structure, one per user */

typedef struct _last_read_s {
    ulong  user_crc;            /* CRC-32 of user name (lowercase)     */
    ulong  user_id;             /* Unique UserID                       */
    ulong  last_read_msg;       /* Last read message number            */
    ulong  high_read_msg;       /* Highest read message number         */
} jam_lastread_t;

typedef struct _search_s {
    ulong msgnum;               /* message number                      */
    ulong offset;               /* offset within header file           */
} jam_search_t;

/*-------------------------------------------------------------*/
/* klass defination                                            */
/*-------------------------------------------------------------*/

typedef struct _jam_s jam_t;

struct _jam_s {
    object_t parent_klass;
    int (*ctor)(object_t *, item_list_t *);
    int (*dtor)(object_t *);
    int (*_compare)(jam_t *, jam_t *);
    int (*_override)(jam_t *, item_list_t *);
    int (*_open)(jam_t *);
    int (*_close)(jam_t *);
    int (*_remove)(jam_t *);
    int (*_lock)(jam_t *);
    int (*_unlock)(jam_t *);
    int (*_size)(jam_t *, ssize_t *);
    int (*_crc32)(jam_t *, uchar *, ulong, ulong *); 

    int (*_read_header)(jam_t *, jam_header_t *);
    int (*_write_header)(jam_t *, jam_header_t *);

    int (*_find_index)(jam_t *, ulong, jam_index_t *);
    int (*_delete_index)(jam_t *, ulong, jam_header_t *);
    int (*_read_index)(jam_t *, jam_index_t *, ssize_t *);
    int (*_write_index)(jam_t *, jam_index_t *, ssize_t *);
    int (*_append_index)(jam_t *, jam_header_t *, jam_index_t *);
    int (*_normalize_index)(jam_t *, jam_index_t *, jam_index_t *);
    int (*_update_index)(jam_t *, ulong, jam_header_t *, jam_index_t *);

    int (*_del_lastread)(jam_t *, ulong);
    int (*_add_lastread)(jam_t *, jam_lastread_t *);
    int (*_get_lastread)(jam_t *, ulong, jam_lastread_t *);
    int (*_put_lastread)(jam_t *, ulong, jam_lastread_t *);
    int (*_find_lastread)(jam_t *, ulong, jam_lastread_t *);
    int (*_read_lastread)(jam_t *, jam_lastread_t *, ssize_t *);
    int (*_write_lastread)(jam_t *, jam_lastread_t *, ssize_t *);
    int (*_append_lastread)(jam_t *, jam_header_t *, jam_lastread_t *);
    int (*_normalize_lastread)(jam_t *, jam_lastread_t *, jam_lastread_t *);
    int (*_update_lastread)(jam_t *, ulong, jam_header_t *, jam_lastread_t *);

    int (*_write_text)(jam_t *, char *, int);
    int (*_read_text)(jam_t *, char **, int);
    int (*_find_text)(jam_t *, off_t, char **, int);
    int (*_append_text)(jam_t *, char *, int, ssize_t *);

    int (*_del_message)(jam_t *, ulong);
    int (*_new_message)(jam_t *, jam_message_t **);
    int (*_read_message)(jam_t *, jam_message_t *, queue *);
    int (*_write_message)(jam_t *, jam_message_t *, queue *);
    int (*_put_message)(jam_t *, ulong, jam_message_t *, queue *);
    int (*_add_message)(jam_t *, jam_message_t *, queue *, char *);
    int (*_find_message)(jam_t *, off_t, jam_message_t *, queue *);
    int (*_append_message)(jam_t *, jam_message_t *, queue *, ssize_t *);
    int (*_get_message)(jam_t *, ulong, jam_message_t *, queue *, char **);
    int (*_normalize_message)(jam_t *, jam_message_t *, queue *, jam_message_t *, queue *);
    int (*_search_messages)(jam_t *, ulong, int, int (*compare)(ulong, jam_message_t *), queue *);

    int (*_free_fields)(jam_t *, queue *);
    int (*_free_field)(jam_t *, jam_field_t *);
    int (*_read_fields)(jam_t *, ulong, queue *);
    int (*_write_fields)(jam_t *, queue *, int *);
    int (*_new_field)(jam_t *, ushort, char *, jam_field_t **);

    files_t *header;
    files_t *text;
    files_t *index;
    files_t *lastread;
    tracer_t *trace;
    int locked;
    ulong base;
    ulong revision;
};

/*-------------------------------------------------------------*/
/* constants                                                   */
/*-------------------------------------------------------------*/

#define JAM(x) ((jam_t *)(x))

#define JAM_K_PATH    1
#define JAM_K_NAME    2
#define JAM_K_RETRIES 3
#define JAM_K_TIMEOUT 4
#define JAM_K_BASE    5
#define JAM_K_TRACE   6

#define JAM_M_DESTRUCTOR         1
#define JAM_M_OPEN               2
#define JAM_M_CLOSE              3
#define JAM_M_REMOVE             4
#define JAM_M_SIZE               5
#define JAM_M_LOCK               6
#define JAM_M_UNLOCK             7
#define JAM_M_READ_HEADER        8
#define JAM_M_WRITE_HEADER       9
#define JAM_M_ADD_LASTREAD       10
#define JAM_M_DEL_LASTREAD       11
#define JAM_M_GET_LASTREAD       12
#define JAM_M_PUT_LASTREAD       13
#define JAM_M_READ_LASTREAD      14
#define JAM_M_WRITE_LASTREAD     15
#define JAM_M_NORMALIZE_LASTREAD 16
#define JAM_M_FIND_INDEX         17
#define JAM_M_APPEND_INDEX       18
#define JAM_M_DELETE_INDEX       19
#define JAM_M_UPDATE_INDEX       20
#define JAM_M_READ_INDEX         21
#define JAM_M_WRITE_INDEX        22
#define JAM_M_NORMALIZE_INDEX    23
#define JAM_M_FIND_TEXT          24
#define JAM_M_APPEND_TEXT        25
#define JAM_M_READ_TEXT          26
#define JAM_M_WRITE_TEXT         27
#define JAM_M_ADD_MESSAGE        28
#define JAM_M_GET_MESSAGE        29
#define JAM_M_PUT_MESSAGE        30
#define JAM_M_DEL_MESSAGE        31
#define JAM_M_NEW_MESSAGE        32
#define JAM_M_APPEND_MESSAGE     33
#define JAM_M_DELETE_MESSAGE     34
#define JAM_M_FIND_MESSAGE       35
#define JAM_M_READ_MESSAGE       36
#define JAM_M_WRITE_MESSAGE      37
#define JAM_M_READ_FIELDS        38
#define JAM_M_WRITE_FIELDS       39
#define JAM_M_FREE_FIELDS        40
#define JAM_M_NEW_FIELD          41
#define JAM_M_SEARCH_MESSAGES    42
#define JAM_M_NORMALIZE_MESSAGE  43

/* file extensions */

#define EXT_HDRFILE ".jhr"
#define EXT_TXTFILE ".jdt"
#define EXT_IDXFILE ".jdx"
#define EXT_LRDFILE ".jlr"

/* crc definations */

#define JAM_NO_CRC 0xffffffff

/* revision level and header signature */

#define CURRENTREBLEV 1
#define HEADERSIGNATURE "JAM\0"

/* message status bits */

#define MSG_LOCAL       0x00000001L /* Msg created locally                   */
#define MSG_INTRANSIT   0x00000002L /* Msg is in-transit                     */
#define MSG_PRIVATE     0x00000004L /* Private                               */
#define MSG_READ        0x00000008L /* Read by addressee                     */
#define MSG_SENT        0x00000010L /* Sent to remote                        */
#define MSG_KILLSENT    0x00000020L /* Kill when sent                        */
#define MSG_ARCHIVESENT 0x00000040L /* Archive when sent                     */
#define MSG_HOLD        0x00000080L /* Hold for pick-up                      */
#define MSG_CRASH       0x00000100L /* Crash                                 */
#define MSG_IMMEDIATE   0x00000200L /* Send Msg now, ignore restrictions     */
#define MSG_DIRECT      0x00000400L /* Send directly to destination          */
#define MSG_GATE        0x00000800L /* Send via gateway                      */
#define MSG_FILEREQUEST 0x00001000L /* File request                          */
#define MSG_FILEATTACH  0x00002000L /* File(s) attached to Msg               */
#define MSG_TRUNCFILE   0x00004000L /* Truncate file(s) when sent            */
#define MSG_KILLFILE    0x00008000L /* Delete file(s) when sent              */
#define MSG_RECEIPTREQ  0x00010000L /* Return receipt requested              */
#define MSG_CONFIRMREQ  0x00020000L /* Confirmation receipt requested        */
#define MSG_ORPHAN      0x00040000L /* Unknown destination                   */
#define MSG_ENCRYPT     0x00080000L /* Msg text is encrypted                 */
#define MSG_COMPRESS    0x00100000L /* Msg text is compressed                */
#define MSG_ESCAPED     0x00200000L /* Msg text is seven bit ASCII           */
#define MSG_FPU         0x00400000L /* Force pickup                          */
#define MSG_TYPELOCAL   0x00800000L /* Msg is for local use only (no export) */
#define MSG_TYPEECHO    0x01000000L /* Msg is for conference distribution    */
#define MSG_TYPENET     0x02000000L /* Msg is direct network mail            */
#define MSG_NODISP      0x20000000L /* Msg may not be displayed to user      */
#define MSG_LOCKED      0x40000000L /* Msg is locked, no editing possible    */
#define MSG_DELETED     0x80000000L /* Msg is deleted                        */

/* message header subfield types */

#define JAMSFLD_OADDRESS    0
#define JAMSFLD_DADDRESS    1
#define JAMSFLD_SENDERNAME  2
#define JAMSFLD_RECVRNAME   3
#define JAMSFLD_MSGID       4
#define JAMSFLD_REPLYID     5
#define JAMSFLD_SUBJECT     6
#define JAMSFLD_PID         7
#define JAMSFLD_TRACE       8
#define JAMSFLD_ENCLFILE    9
#define JAMSFLD_ENCLFWALIAS 10
#define JAMSFLD_ENCLFREQ    11
#define JAMSFLD_ENCLFILEWC  12
#define JAMSFLD_ENCLINDFILE 13
#define JAMSFLD_EMBINDAT    1000
#define JAMSFLD_FTSKLUDGE   2000
#define JAMSFLD_SEENBY2D    2001
#define JAMSFLD_PATH2D      2002
#define JAMSFLD_FLAGS       2003
#define JAMSFLD_TZUTCINFO   2004
#define JAMSFLD_UNKNOWN     0xFFFF

/*-------------------------------------------------------------*/
/* interface                                                   */
/*-------------------------------------------------------------*/

extern jam_t *jam_create(char *, char *, int, int, int, tracer_t *);
extern int jam_destroy(jam_t *);
extern int jam_compare(jam_t *, jam_t *);
extern int jam_override(jam_t *, item_list_t *);
extern char *jam_version(jam_t *);

extern int jam_open(jam_t *);
extern int jam_close(jam_t *);
extern int jam_remove(jam_t *);
extern int jam_size(jam_t *, ssize_t *);
extern int jam_crc32(jam_t *, uchar *, ulong, ulong *);

extern int jam_del_lastread(jam_t *, ulong);
extern int jam_add_lastread(jam_t *, ulong, ulong);
extern int jam_get_lastread(jam_t *, ulong, jam_lastread_t *);
extern int jam_put_lastread(jam_t *, ulong, jam_lastread_t *);

extern int jam_del_message(jam_t *, ulong);
extern int jam_free_field(jam_t *, jam_field_t *);
extern int jam_new_message(jam_t *, jam_message_t **);
extern int jam_new_field(jam_t *, ushort, char *, jam_field_t **);
extern int jam_put_message(jam_t *, ulong, jam_message_t *, queue *);
extern int jam_add_message(jam_t *, jam_message_t *, queue *, char *);
extern int jam_get_message(jam_t *, ulong, jam_message_t *, queue *, char **);
extern int jam_search_messages(jam_t *, ulong, int, int (*compare)(ulong, jam_message_t *), queue *);

#endif

