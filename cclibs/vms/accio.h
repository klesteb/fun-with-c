
/*---------------------------------------------------------------------------*/
/*  Copyright (c) 1999 by Kevin L. Esteb                                     */
/*                                                                           */
/*  Permission to use, copy, modify, and distribute this software and its    */
/*  documentation for any purpose and without fee is hereby granted,         */
/*  provided that the above copyright notice appears in all copies. The      */
/*  author makes no representations about the suitability of this software   */
/*  for any purpose. It is provided "as is" without express or implied       */
/*  warranty.                                                                */
/*---------------------------------------------------------------------------*/

#ifndef ACCIO_H
#define ACCIO_H 1

#define __NEW_STARLET
#include <acrdef.h>
#undef __NEW_STARLET

#include "vms.h"
#include "rmsio.h"

#define ACCBUF 512

typedef struct {
    ULONG pid;                    /* PROCESS ID                       */
    ULONG owner;                  /* OWNER PROCES ID                  */
    ULONG uic;                    /* PROCESS UIC                      */
    QUADWORD priv;                /* PROCESS PRIV                     */
    UBYTE pri;                    /* PROCESS PRIORITY                 */
    UBYTE idflgs;                 /* FLAGS BIT MASK                   */
    ULONG jobid;                  /* JOB ID                           */
    char *username;               /* USERNAME OFFSET                  */
    char *account;                /* ACCOUNT NAME OFFSET              */
    char *nodename;               /* NODE NAME OFFSET                 */
    char *terminal;               /* TERMINAL NAME OFFSET             */
    char *jobname;                /* JOB NAME OFFSET                  */
    char *queue;                  /* QUEUE NAME OFFSET                */
    long *nodeaddr;               /* REMOTE NODE ADDRESS              */
    char *remoteid;               /* REMOTE ID OFFSET                 */
    char *fulladdr;               /* REMOTE PHASE V ADDRESS           */
    char *fullname;               /* REMOTE PHASE V FULLNAME          */
} ACR_IDENT;

typedef struct {
    QUADWORD login;               /* PROCESS/IMAGE START TIME           */
    ULONG status;                 /* PROCESS/IMAGE FINAL STATUS         */
    ULONG imgcnt;                 /* IMAGE EXECUTION COUNT/SEQUENCE NUMBER */
    ULONG cputime;                /* PROCESS/IMAGE CPU TIME             */
    ULONG faults;                 /* PROCESS/IMAGE PAGE FAULT COUNT     */
    ULONG faultio;                /* PROCESS/IMAGE PAGE FAULT I/O COUNT */ 
    ULONG wspeak;                 /* PROCESS/IMAGE WORKING SET PEAK     */
    ULONG pagefl;                 /* PROCESS/IMAGE PEAK PAGE FILE USAGE */ 
    ULONG diocnt;                 /* PROCESS/IMAGE DIRECT I/O COUNT     */
    ULONG biocnt;                 /* PROCESS/IMAGE BUFFERED I/O COUNT   */
    ULONG volumes;                /* PROCESS/IMAGE VOLUME MOUNT COUNT   */
    ULONG vp_cputime;             /* PROCESS/IMAGE VECTOR CPU TIME      */
} ACR_RESOURCE;

typedef struct {
    char *imagename;              /* IMAGENAME                          */
} ACR_IMAGENAME;

typedef struct {
    ULONG printsts;               /* JOB STATUS                         */
    QUADWORD quetime;             /* TIME JOB WAS QUEUED                */
    QUADWORD begtime;             /* TIME JOB WAS BEGUN                 */
    ULONG symcputim;              /* SYMBIONT CPU TIME                  */
    ULONG pagecnt;                /* TOTAL PAGES PRINTED                */
    ULONG qiocnt;                 /* TOTAL QIOS ISSUED                  */
    ULONG getcnt;                 /* TOTAL GETS ISSUED                  */
} ACR_PRINT;

typedef struct {
    char *filename;               /* FILENAME                           */
} ACR_FILENAME;

typedef struct {
    int length;
    void *userdata;               /* USER DATA                          */
} ACR_USERDATA;

typedef union {
    ACRDEF x;
    ACRDEF1 y;
} ACR_HEADER;

RFILE *acc_open(char *filename);
void acc_close(RFILE *rp);
int  acc_read_record(RFILE *rp, void *buffer, int buflen);
int  acc_get_header(void *buffer, int buflen, int *length, int *type, int *subtype, QUADWORD *systime);
void *acc_get_packet(void *buffer, int buflen, int offset, int *length, int *type, int *subtype);
int  acc_decode_packet(int type, int subtype, void *packet, void *record);
void acc_free_record(int type, int subtype, void *record);
void acc_free_packet(int type, int subtype, void *packet);

#endif

