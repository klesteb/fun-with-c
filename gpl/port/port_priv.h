
/*---------------------------------------------------------------------------*/
/*                                                                           */
/* TITLE:       port_priv.h                                                  */
/*                                                                           */
/* FACILITY:    Private variables and structures for the PORT package.       */
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

#ifndef PORT_PRIV_H
#define PORT_PRIV_H 1

#include  <errno.h>             /* System error definitions.            */
#include  <stdio.h>             /* Standard I/O definitions.            */
#include  <stdlib.h>            /* Standard C library definitions.      */

#include  "gpl/nix_util.h"      /* Network I/O Handler definitions.     */
#include  "gpl/str_util.h"      /* String manipulation functions.       */
#include  "gpl/tcp_util.h"      /* TCP/IP network utilities.            */
#include  "gpl/vperror.h"       /* VPERROR() definitions.               */
#include  "gpl/port_util.h"     /* Listening port utilities.            */

/*----------------------------------------------------------------------*/
/* Listening Port - represents a network port at which a server listens */
/* for connection requests from clients.                                */
/*----------------------------------------------------------------------*/

typedef struct _ListeningPort {
    char          *name;        /* Server name.                         */
    ClientCreateFunc createF;   /* Function to create a client.         */
    void          *parameter;   /* Arbitrary parameter passed to above. */
    TcpEndpoint   endpoint;     /* Listening socket.                    */
    NxAppContext  context;      /* NIX dispatcher context.              */
    NxInputId     sourceID;     /* ID of registered listening port.     */
}  _ListeningPort;

/*----------------------------------------------------------------------*/
/* Private functions                                                    */
/*----------------------------------------------------------------------*/

extern int port_answer(
#    if __STDC__
        NxAppContext  appl_context,
        NxInputId  source_ID,
        int  source,
        void  *client_data
#    endif
    );

#endif

