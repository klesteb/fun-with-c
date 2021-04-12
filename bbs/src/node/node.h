
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

#include "rms.h"
#include "tracer.h"
#include "datatypes.h"

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
    off_t  record;              /* Index of record                   */
} node_search_t;

/*-------------------------------------------------------------*/
/* klass interface                                             */
/*-------------------------------------------------------------*/

extern rms_t *node_create(char *, int, int, int, tracer_t *);
extern char *node_version(rms_t *);

#define node_destroy(self) rms_destroy(self)
#define node_compare(self, other) rms_compare(self, other)
#define node_override(self, items) rms_override(self, items)

#define node_open(self) rms_open(self)
#define node_close(self) rms_close(self);
#define node_del(self, recnum) rms_del(self, recnum)
#define node_extend(self, amount) rms_extend(self, amount)
#define node_add(self, node) rms_add(self, (void *)node)
#define node_get(self, recnum, node) rms_get(self, recnum, (void *)node)
#define node_put(self, recnum, node) rms_put(self, recnum, (void *)node)
#define node_find(self, data, len, compare, recnum) rms_find(self, data, len, compare, recnum)
#define node_search(self, data, len, compare, capture, results) rms_search(self, data, len, compare, capture, results)

#endif

