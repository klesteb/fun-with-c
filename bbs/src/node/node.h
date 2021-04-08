
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
#include "que_util.h"
#include "datatypes.h"
#include "item_list.h"

/*-------------------------------------------------------------*/
/* constants                                                   */
/*-------------------------------------------------------------*/

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
#define NODE_OFFL 26            /* Node is offline                       */

/*-------------------------------------------------------------*/
/* data structures                                             */
/*-------------------------------------------------------------*/

typedef struct _node_base_s {   /* Node information kept in node.dat */
    uchar  action;              /* Action User is doing on Node      */
    ushort useron;              /* User on Node                      */
    long   nodenum;             /* Node number                       */
    int    revision;            /* Revision level of record          */
} node_base_t;

typedef struct _node_search_s {
    ushort useron;              /* User on Node                      */
    long   nodenum;             /* Node number                       */
    int    index;               /* Index of record                   */
} node_search_t;

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
    int (*_extend)(node_t *, int);
    int (*_get_sequence)(node_t *, long *);
    int (*_get)(node_t *, int, node_base_t *);
    int (*_put)(node_t *, int, node_base_t *);
    int (*_next)(node_t *, node_base_t *, ssize_t *);
    int (*_prev)(node_t *, node_base_t *, ssize_t *);
    int (*_last)(node_t *, node_base_t *, ssize_t *);
    int (*_read)(node_t *, node_base_t *, ssize_t *);
    int (*_write)(node_t *, node_base_t *, ssize_t *);
    int (*_first)(node_t *, node_base_t *, ssize_t *);
    int (*_build)(node_t *, node_base_t *, node_base_t *);
    int (*_normalize)(node_t *, node_base_t *, node_base_t *);
    int (*_find)(node_t *, void *, int, int (*compare)(void *, int, node_base_t *), int *);
    int (*_search)(node_t *, void *, int, int (*compare)(void *, int, node_base_t *), queue *);

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
#define NODE_M_EXTEND     14
#define NODE_M_NORMALIZE  15
#define NODE_M_FIND       16
#define NODE_M_SEARCH     17

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
extern int node_extend(node_t *, int);
extern int node_index(node_t *, int *);
extern int node_get(node_t *, int, node_base_t *);
extern int node_put(node_t *, int, node_base_t *);
extern int node_find(node_t *, void *, int, int (*compare)(void *, int, node_base_t *), int *);
extern int node_search(node_t *, void *, int, int (*compare)(void *, int, node_base_t *), queue *);

#endif

