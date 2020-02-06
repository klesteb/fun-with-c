
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
/*                                                                           */
/* TITLE:       rmsio.h                                                      */
/*                                                                           */
/* FACILITY:    Global variables and structures for the RMSIO package.       */
/*                                                                           */
/* ABSTRACT:    These are the declarations of the data structures and        */
/*              routines to implement the RMSIO functions.                   */
/*                                                                           */
/* ENVIRONMENT: VMS                                                          */
/*                                                                           */
/* Version      Author                                           Date        */
/* -------      ---------------------------------------------    ----------- */
/* 1-000        Kevin Esteb                                      20-Dec-1999 */
/*                                                                           */
/*---------------------------------------------------------------------------*/

#ifndef RMSIO_H
#define RMSIO_H 1

/*----------------------------------------------------------------------*/
/*    RMSIO Structures and Definitions.                                 */
/*----------------------------------------------------------------------*/

#ifndef RFILE_T
#define RFILE void     /* make structure opaque */
#endif

enum {
    RMS_O_ACCESS,
    RMS_O_ALLOW,
    RMS_O_END
} RMS_O_OPTIONS;

typedef struct _rhb {
    UBYTE prefix;
    UBYTE postfix;
} rhb;

typedef struct _fid {
    char t_dvi[16];
    UWORD w_fid[3];
    UWORD w_did[3];
} fid;

/*----------------------------------------------------------------------*/
/*    Miscellaneous declarations.                                       */
/*----------------------------------------------------------------------*/

/* Access flags for rms_open().                                         */

#define RMS_K_ACCESS_APPEND 0
#define RMS_K_ACCESS_READ   1
#define RMS_K_ACCESS_WRITE  2
#define RMS_K_ACCESS_UPDATE 3

/* Allow flags for rms_open().                                          */

#define RMS_K_ALLOW_NONE    0
#define RMS_K_ALLOW_READ    1
#define RMS_K_ALLOW_WRITE   2
#define RMS_K_ALLOW_UPDATE  3

/* File attributes for carraige control.                                */

#define RMS_K_RAT_CR  0
#define RMS_K_RAT_FTN 1
#define RMS_K_RAT_PRN 2

/* File organizations.                                                  */

#define RMS_K_ORG_SEQ 0
#define RMS_K_ORG_REL 1
#define RMS_K_ORG_IDX 3
#define RMS_K_ORG_UDF 4

/* File record format.                                                   */

#define RMS_K_RFM_VAR   0
#define RMS_K_RFM_FIX   1
#define RMS_K_RFM_STM   2
#define RMS_K_RFM_STMCR 3
#define RMS_K_RFM_STMLF 4
#define RMS_K_RFM_UDF   5
#define RMS_K_RFM_VFC   6

/*----------------------------------------------------------------------*/
/*    Public functions.                                                 */
/*----------------------------------------------------------------------*/

/* Record I/O operations.                                               */

extern RFILE *rms_open(char *fname, int params, ...);
extern RFILE *rms_open_fid(fid fid, int params, ...);
extern int rms_delete(RFILE *fp);
extern int rms_rewind(RFILE *fp);
extern int rms_getrel(RFILE *fp, void *ptr, size_t bufsize, size_t key, size_t *len);
extern int rms_getidx(RFILE *fp, void *buf, size_t bufsize, int index,
                      void *key, int key_size, size_t *len);
extern int rms_getseq(RFILE *fp, void *ptr, size_t bufsize, size_t *len);
extern int rms_put(RFILE *fp, void *ptr, size_t buffsize, size_t *wrote);
extern int rms_update(RFILE *fp, void *buf, size_t bufsize);

/* Block I/O operations.                                                */

extern RFILE *rms_open_blk(char *fname, int params, ...);
extern int rms_read(RFILE *fp, void *buf, size_t bufsize, size_t *len);
extern int rms_write(RFILE *fp,  void *buf, size_t bufsize, size_t *wrote);

/* Common RMS routines.                                                 */

extern int rms_flush(RFILE *fp);
extern int rms_close(RFILE *fp);
extern int rms_error(RFILE *fp);
extern int rms_create(char *fld_str, char *fname);
extern int rms_check_access(char *fname, int uic);
extern int rms_file_org(RFILE *fp);
extern int rms_file_rat(RFILE *fp);
extern int rms_file_rfm(RFILE *fp);
extern void rms_get_rhb(RFILE *fp, rhb *rhb);
extern int rms_file_size_bytes(RFILE *fp);
extern int rms_file_size_blocks(RFILE *fp);

#endif

