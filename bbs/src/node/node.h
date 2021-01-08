
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

#ifndef _NODE_H
#define _NODE_H

#include "files.h"
#include "tracer.h"
#include "object.h"
#include "datatypes.h"
#include "item_list.h"

/*-------------------------------------------------------------*/
/* constants                                                   */
/*-------------------------------------------------------------*/

/* status value for node.status */

#define NODE_WFC           1    /* Waiting for Call                      */
#define NODE_LOGON         2    /* at logon prompt                       */
#define NODE_NEWUSER       3    /* New user applying                     */
#define NODE_INUSE         4    /* In Use                                */
#define NODE_QUIET         5    /* In Use - quiet mode                   */
#define NODE_OFFLINE       6    /* Offline                               */
#define NODE_NETTING       7    /* Networking                            */
#define NODE_EVENT_WAITING 8    /* Waiting for all nodes to be inactive  */
#define NODE_EVENT_RUNNING 9    /* Running an external event             */
#define NODE_EVENT_LIMBO   10   /* Allowing another node to run an event */

/* bit values for node.misc */

#define NODE_ANON   (1L<<0)     /* Anonymous User                        */
#define NODE_LOCK   (1L<<1)     /* Locked for sysops only                */
#define NODE_INTR   (1L<<2)     /* Interrupted - hang up                 */
#define NODE_UMSG   (1L<<3)     /* Message is waiting                    */
#define NODE_POFF   (1L<<4)     /* Page disabled                         */
#define NODE_AOFF   (1L<<5)     /* Activity Alert disabled               */
#define NODE_UDAT   (1L<<6)     /* User data has been updated            */
#define NODE_RRUN   (1L<<7)     /* Re-run this node when log off         */
#define NODE_EVENT  (1L<<8)     /* Must run node event after log off     */
#define NODE_DOWN   (1L<<9)     /* Down this node after logoff           */
#define NODE_RPCHT  (1L<<10)    /* Reset private chat                    */
#define NODE_NMSG   (1L<<11)    /* Node message waiting                  */
#define NODE_EXT    (1L<<12)    /* Extended info on node action          */
#define NODE_WMSG   (1L<<13)    /* Wall message is waiting               */

/* values for node.action */

#define NODE_MAIN 1             /* Main Prompt                           */
#define NODE_RMSG 2             /* Reading Messages                      */
#define NODE_RMAL 3             /* Reading Mail                          */
#define NODE_SMAL 4             /* Sending Mail                          */
#define NODE_RTXT 5             /* Reading G-Files                       */
#define NODE_RSML 6             /* Reading Sent Mail                     */
#define NODE_PMSG 7             /* Posting Message                       */
#define NODE_AMSG 8             /* Auto-message                          */
#define NODE_XTRN 9             /* Running External Program              */
#define NODE_DFLT 10            /* Main Defaults Section                 */
#define NODE_XFER 11            /* Transfer Prompt                       */
#define NODE_DLNG 12            /* Downloading File                      */
#define NODE_ULNG 13            /* Uploading File                        */
#define NODE_BXFR 14            /* Bidirectional Transfer                */
#define NODE_LFIL 15            /* Listing Files                         */
#define NODE_LOGN 16            /* Logging on                            */
#define NODE_LCHT 17            /* In Local Chat with Sysop              */
#define NODE_MCHT 18            /* In Multi-Chat with Other Nodes        */
#define NODE_GCHT 19            /* In Local Chat with Guru               */
#define NODE_CHAT 20            /* In Chat Section                       */
#define NODE_SYSP 21            /* Sysop Activity                        */
#define NODE_TQWK 22            /* Transferring QWK packet               */
#define NODE_PCHT 23            /* In Private Chat                       */
#define NODE_PAGE 24            /* Paging another node for Private Chat  */
#define NODE_RFSD 25            /* Retrieving file from seq dev (aux=dev)*/

/*-------------------------------------------------------------*/
/* data structures                                             */
/*-------------------------------------------------------------*/

typedef struct _node_base_s {   /* Node information kept in NODE.DAB */
    uchar status;               /* Current Status of Node            */
    uchar errors;               /* Number of Critical Errors         */
    uchar action;               /* Action User is doing on Node      */
    uchar pad1;                 /* alignment padding                 */
    ushort useron;              /* User on Node                      */
    ushort connection;          /* Connection rate of Node           */
    ushort misc;                /* Miscellaneous bits for node       */
    ushort aux;                 /* Auxillary word for node           */
    ulong  extaux;              /* Extended aux dword for node       */
    long   msgnum;              /* Message number                    */
} node_base_t;

/*-------------------------------------------------------------*/
/* klass defination                                            */
/*-------------------------------------------------------------*/

typedef struct _node_s node_t;

struct _node_s {
    object_t parent_klass;
    int (*ctor)(object_t *, item_list_t *);
    int (*dtor)(object_t *);
    int (*_compare)(node_t *, node_t *);
    int (*_override)(node_t *, item_list_t *);

    int (*_open)(node_t *);
    int (*_close)(node_t *);
    int (*_unlock)(node_t *);
    int (*_lock)(node_t *, off_t);
    int (*_get_sequence)(node_t *, long *);
    int (*_get)(node_t *, int, node_base_t *);
    int (*_put)(node_t *, int, node_base_t *);
    int (*_put_message)(node_t *, char *, long *);
    int (*_get_message)(node_t *, long, char **);
    int (*_next)(node_t *, node_base_t *, ssize_t *);
    int (*_prev)(node_t *, node_base_t *, ssize_t *);
    int (*_last)(node_t *, node_base_t *, ssize_t *);
    int (*_read)(node_t *, node_base_t *, ssize_t *);
    int (*_write)(node_t *, node_base_t *, ssize_t *);
    int (*_first)(node_t *, node_base_t *, ssize_t *);
    int (*_build)(node_t *, node_base_t *, node_base_t *);

    int index;
    int nodes;
    int locked;
    int retries;
    int timeout;
    char *path;
    files_t *nodedb;
    files_t *sequence;
    tracer_t *trace;
};

/*-------------------------------------------------------------*/
/* klass constants                                             */
/*-------------------------------------------------------------*/

#define NODE(x) ((node_t *)(x))

#define NODE_K_PATH    1
#define NODE_K_RETRIES 2
#define NODE_K_TIMEOUT 3
#define NODE_K_TRACE   4
#define NODE_K_NODES   5

#define NODE_M_DESTRUCTOR 1
#define NODE_M_OPEN       2
#define NODE_M_CLOSE      3
#define NODE_M_UNLOCK     4
#define NODE_M_LOCK       5
#define NODE_M_GET        6
#define NODE_M_PUT        7
#define NODE_M_NEXT       8
#define NODE_M_PREV       9
#define NODE_M_LAST       10
#define NODE_M_WRITE      11
#define NODE_M_FIRST      12
#define NODE_M_BUILD      13

/*-------------------------------------------------------------*/
/* klass interface                                             */
/*-------------------------------------------------------------*/

extern node_t *node_create(char *, int, int, int, tracer_t *);
extern int node_destroy(node_t *);
extern int node_compare(node_t *, node_t *);
extern int node_override(node_t *, item_list_t *);
extern char *node_version(node_t *);

extern int node_open(node_t *);
extern int node_close(node_t *);
extern int node_index(node_t *, int *);
extern int node_get(node_t *, int, node_base_t *);
extern int node_put(node_t *, int, node_base_t *);
extern int node_get_message(node_t *, long, char **);
extern int node_put_message(node_t *, char *, long *);
extern int node_next(node_t *, node_base_t *, ssize_t *);
extern int node_prev(node_t *, node_base_t *, ssize_t *);
extern int node_last(node_t *, node_base_t *, ssize_t *);
extern int node_first(node_t *, node_base_t *, ssize_t *);

#endif

