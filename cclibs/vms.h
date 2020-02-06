
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

#ifndef VMS_H
#define VMS_H

#include <stddef.h>
#include "que_util.h"

#ifndef TRUE
#define TRUE  1
#define FALSE 0
#endif

#define SYMSIZ 255                  /* DCL - Max symbol size.           */
#define LNMSIZ 255                  /* DCL - Max logical name size.     */
#define CLISIZ 1024                 /* DCL - command line length.       */

#define DCL_WARNING     0x00000000
#define DCL_SUCCESS     0x10000001
#define DCL_ERROR       0x10000002
#define DCL_INFORMATION 0x10000003
#define DCL_FATAL_ERROR 0x10000004

/* Create some new data types. Mainly for easier reading.               */

typedef char               BYTE;
typedef unsigned char      UBYTE;
typedef short int          WORD;
typedef unsigned short int UWORD;
typedef long               LONG;
typedef unsigned long      ULONG;

/* System service data structures.                                      */

typedef struct _ITEM_LIST_3 {
    UWORD buffer_length;
    UWORD item_code;
    void *buffer_address;
    void *return_address;
} ITEM_LIST_3;

typedef struct _ITEM_LIST_2 {
    WORD comp_length;
    WORD code;
    long comp_address;
} ITEM_LIST_2;

typedef struct _QUADWORD {
    ULONG junk[2];
} QUADWORD;

typedef struct _ECB {
    ULONG flink;
    ULONG (*handler)(ULONG *, void *);
    ULONG arg_count;
    ULONG *exit_status;
    void  *data;
} ECB;

typedef struct _LKSB {
    UWORD stat;
    UWORD reserv;
    ULONG lock_id;
    char value[16];
} LKSB;

typedef struct _TIMBUF {            /* The time buffer returned from    */
    UWORD year;                     /* sys$numtim().                    */
    UWORD month;
    UWORD day;
    UWORD hour;
    UWORD minutes;
    UWORD seconds;
    UWORD hundredths;
} TIMBUF;

typedef union _IOSB {
    struct {                        /* QIO Completion status            */
        UWORD stat;                 /* I/O completion status            */
        UWORD byte_count;           /* read/write qio byte count        */
        ULONG junk;                 /* filler                           */
    } io;
    struct {                        /* LAT QIO Completion status        */
        UWORD stat;                 /* I/O completion status            */
        union {
            UWORD reason_code;      /* LT_CONNECT only                  */
            UWORD byte_count;       /* read/write qio byte count        */
        } variable_info;
        ULONG junk;                 /* filler                           */
    } lat;
    struct {                        /* SNDJBC/GETQUI Completion status  */
        ULONG stat;                 /* I/O Completion status            */
        ULONG junk;                 /* filler                           */
    } jbc;
    struct {                        /* Global Sections                  */
        UWORD stat;                 /* QIO status.                      */
        UWORD dev_stat;             /* Device status.                   */
        ULONG address;              /* Address of first page not written. */
    } gsc;
    QUADWORD quad;
} IOSB;

/* Carriage control - used in print symbionts.                          */

typedef struct {
    char trailing_char;              /* trailing carriage control (cr)  */
    char trailing_cnt;               /* trailing carriage control count */
    char leading_char;               /* leading carriage control  (lf)  */
    char leading_cnt;                /* leading carriage control count  */
} carcon_t;

/* Data structure for vms_submit().                                     */

typedef struct {
    int keep_sw;
    int hold_sw;
    int notify_sw;
    int log_print_sw;
    int del_file_sw;
    char *after;
    char *logname;
    char *logqueue;
    char *username;
    char *p1;
    char *p2;
    char *p3;
    char *p4;
    char *p5;
    char *p6;
    char *p7;
    char *p8;
} submit_t;

typedef struct _IFILE {              /* Event driven I/O.               */
    IOSB iosb;
    ULONG ef;
    ULONG timer_ef;
    WORD lun;
    WORD qued;
    int  bufsiz;
    void *buffer;
} IFILE;

typedef struct _SUBPROC {            /* Subprocess data structure       */
    IFILE *ifd;
    IFILE *ofd;
    long spawn_id;
    long cli_active;
    ULONG cli_status;
    ECB ecb;
} SUBPROC;

enum {                               /* Event flag wait modes           */
    ef_a_or = 1,
    ef_a_and
} ef_styles;


#define SET_ECB(x,a,b,c) {(x).flink = 0;\
                          (x).handler = (a);\
                          (x).arg_count = 2;\
                          (x).exit_status = (b);\
                          (x).data = (c);}

#define SET_SDESC(x,p,l) {(x).dsc$b_dtype=DSC$K_DTYPE_T;\
                          (x).dsc$b_class=DSC$K_CLASS_S;\
                          (x).dsc$w_length=(l);\
                          (x).dsc$a_pointer=(p);}

#define SET_ITEM2(x,a,b,c) {(x).code=(a);\
                            (x).comp_address=(b);\
                            (x).comp_length=(c);}

#define SET_ITEM3(x,a,b,c,d) {(x).item_code=(a);\
                              (x).buffer_address=(b);\
                              (x).buffer_length=(c);\
                              (x).return_address=(d);}

#define vms_check_status(x) if (((x) & 1) == 0) lib$stop((x));

/*-Prototypes-----------------------------------------------------------*/

void  vms_trnlnm(char *, char *, char *, long, char *);
void  vms_dellnm(char *, char *, UBYTE);
void  vms_crelnm(long, char *, char *, char *, UBYTE, long);
int   vms_chklnm(char *, char *, long);
void  vms_bintim(char *, QUADWORD *);
void  vms_asctim(QUADWORD *, char *, int);
int   vms_cmptim(QUADWORD *, QUADWORD *);
void  vms_numtim(QUADWORD *, TIMBUF *);
void  vms_enable_asts(void);
void  vms_disable_asts(void);
ULONG vms_set_ef_mask(ULONG, ULONG *);
ULONG vms_get_ef(void);
void  vms_set_ef(ULONG);
void  vms_free_ef(ULONG);
void  vms_clear_ef(ULONG);
int   vms_check_ef(ULONG);
int   vms_wait_ef(ULONG, ULONG, int);
void  vms_sndopr(char *, char *);
void  vms_sndopr_req(char *, char *, char *, int);
int   vms_print(char *, char *, char *, long);
int   vms_submit(char *, char *, submit_t *);
int   vms_filescan(char *, char *, char *);
int   vms_file_exists(char *);
void  vms_fmtuic(long, char *, int);
void  vms_getdvi(char *, WORD, int, char *);
void  vms_find_file(char *, void *);
void  vms_mail(char *, char *, char *, char *, void *);
int   vms_init_cli(void *, char *);
int   vms_queio(IFILE *);
int   vms_chkio(IFILE *);
int   vms_create_subprocess(SUBPROC *);
int   vms_execute_command(SUBPROC *, char *, queue *);
void  vms_delete_subprocess(SUBPROC *);
int   vms_tempfile(char *, char *, int);

#endif

