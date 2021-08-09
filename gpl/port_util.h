
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                                                                           */
/* TITLE:       port_util.h                                                  */
/*                                                                           */
/* FACILITY:    Global variables and structures for the PORT package.        */
/*                                                                           */
/* ABSTRACT:    These are the declarations of the data structures and        */
/*              routines to implement the PORT utilities.                    */
/*                                                                           */
/* ENVIRONMENT: VMS/AIX/Linux                                                */
/*                                                                           */
/* Version      Author                                           Date        */
/* -------      ---------------------------------------------    ----------- */
/* 1-000        Kevin Esteb                                      23-Apr-2002 */
/*                                                                           */
/*---------------------------------------------------------------------------*/

#ifndef PORT_UTIL_H
#define PORT_UTIL_H 1

#ifdef __cplusplus
    extern  "C" {
#endif

#include "gpl/ansi_setup.h"         /* ANSI or non-ANSI C?              */
#include "gpl/nix_util.h"			/* Network I/O Handler definitions. */
#include "gpl/tcp_util.h"			/* TCP/IP network utilities.        */

/*----------------------------------------------------------------------*/
/*    Structures and Definitions.                                       */
/*----------------------------------------------------------------------*/

typedef struct _ListeningPort *ListeningPort;

typedef int (*ClientCreateFunc) P_((TcpEndpoint connection,
                                    NxAppContext context,
                                    void *parameter,
                                    void **client));

/*----------------------------------------------------------------------*/
/*    Miscellaneous declarations.                                       */
/*----------------------------------------------------------------------*/

extern int port_util_debug;	     /* Global debug switch (1/0 = yes/no). */

/*----------------------------------------------------------------------*/
/*    Public Functions                                                  */
/*----------------------------------------------------------------------*/

extern  int  port_create P_((const char *name,
                            NxAppContext context,
                            ClientCreateFunc clientCreateF,
                            void *clientParameter,
                            ListeningPort *port));

extern  int  port_destroy P_((ListeningPort port));

#ifdef __cplusplus
    }
#endif

#endif

