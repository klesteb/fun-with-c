
/*---------------------------------------------------------------------------*/
/*                                                                           */
/* TITLE:       lpd_util.h                                                   */
/*                                                                           */
/* FACILITY:    Global variables and structures for the LPD package.         */
/*                                                                           */
/* ABSTRACT:    These are the declarations of the data structures and        */
/*              routines to implement the LPD client. The LPD client is      */
/*              based on RFC-1179; Line Printer Protocol.                    */
/*                                                                           */
/* ENVIRONMENT: VMS/AIX/Linux                                                */
/*                                                                           */
/* Version      Author                                           Date        */
/* -------      ---------------------------------------------    ----------- */
/* 1-000        Kevin Esteb                                      09-Apr-2002 */
/*                                                                           */
/*---------------------------------------------------------------------------*/

#ifndef LPD_UTIL_H
#define LPD_UTIL_H 1

#ifdef __cplusplus
    extern  "C" {
#endif

#include "gpl/ansi_setup.h"         /* ANSI or non-ANSI C?              */
#include "gpl/tcp_util.h"           /* TCP/IP networking utilities.     */
#include "gpl/lfn_util.h"
#include "cclibs/que_util.h"

/*----------------------------------------------------------------------*/
/*    LPD Structures (Client View) and Definitions.                     */
/*----------------------------------------------------------------------*/

typedef struct {
    unsigned short code;
    unsigned long buf_len;
	unsigned long *ret_len;
    void *buffer;
} item_list;

typedef struct _LpdHandle *LpdHandle;       /* LPD handle.              */

/*----------------------------------------------------------------------*/
/*    Miscellaneous declarations.                                       */
/*----------------------------------------------------------------------*/

extern int lpd_util_debug;       /* Global debug switch (1/0 = yes/no). */

#define SET_ITEM(x,a,b,c,d) {(x).code=(a);\
                             (x).buffer=(b);\
                             (x).buf_len=(c);\
                             (x).ret_len=(d);}
 
#define LPD_K_LONG     1         /* Print job status in long format.    */
#define LPD_K_SHORT    2         /* Print job status in short format.   */

#define LPD_QUEUE      1
#define LPD_AGENT      2
#define LPD_JOBID      3
#define LPD_USER       4
#define LPD_ABORT      5
#define LPD_CLASS      6
#define LPD_HOST       7
#define LPD_INDENT     8
#define LPD_JOBNAM     9
#define LPD_BANNER     10
#define LPD_MAIL       11
#define LPD_FILNAM     12
#define LPD_USERID     13
#define LPD_SYMLNK     14
#define LPD_TITLE      15
#define LPD_UNLINK     16
#define LPD_WIDTH      17
#define LPD_PRT_TROFFR 18
#define LPD_PRT_TROFFI 19
#define LPD_PRT_TROFFB 20
#define LPD_PRT_TROFFS 21
#define LPD_PRT_CIF    22
#define LPD_PRT_DVI    23
#define LPD_PRT_TEXT   24
#define LPD_PRT_PLOT   25
#define LPD_PRT_PRTCTL 26
#define LPD_PRT_DTROFF 27
#define LPD_PRT_POST   28
#define LPD_PRT_PR     29
#define LPD_PRT_FOR    30
#define LPD_PRT_TROFF  31
#define LPD_PRT_RASTER 32
#define LPD_PALL       33

/*----------------------------------------------------------------------*/
/*    Public functions.                                                 */
/*----------------------------------------------------------------------*/

extern int lpd_open P_((char *host, char *port, 
                        double timeout, LpdHandle *handle));

extern int lpd_close P_((LpdHandle));

extern int lpd_eof P_((LpdHandle));

extern int lpd_wait_ack P_((LpdHandle));

extern int lpd_put P_((LpdHandle, int, void *));

extern int lpd_print_jobs P_((LpdHandle, char *));

extern int lpd_start_job P_((LpdHandle, char *));

extern int lpd_start_data P_((LpdHandle, char *, int, int));

extern int lpd_end_data P_((LpdHandle));

extern int lpd_end_job P_((LpdHandle));

extern int lpd_send_control P_((LpdHandle, char *, int, item_list *));

extern int lpd_get_stat P_((LpdHandle, char *, int, item_list *, queue *));

extern int lpd_remove_job P_((LpdHandle, char *, char *, int));

#ifdef __cplusplus
    }
#endif

#endif

