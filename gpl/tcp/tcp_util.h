/*
@(#)  FILE: tcp_util.h  RELEASE: 1.7  DATE: 11/05/96, 15:03:41
*/
/*******************************************************************************

    tcp_util.h

    TCP Utility Definitions.

*******************************************************************************/

#ifndef  TCP_UTIL_H             /* Has the file been INCLUDE'd already? */
#define  TCP_UTIL_H  yes

#ifdef __cplusplus
    extern  "C" {
#endif


#include  "ansi_setup.h"                /* ANSI or non-ANSI C? */
#include  "skt_util.h"                  /* Socket support functions. */


/*******************************************************************************
    TCP Network Endpoint (Client View) and Definitions.
*******************************************************************************/

					/* Endpoint handle. */
typedef  struct  _TcpEndpoint  *TcpEndpoint ;


/*******************************************************************************
    Miscellaneous declarations.
*******************************************************************************/

extern  int  tcp_util_debug ;           /* Global debug switch (1/0 = yes/no). */


/*******************************************************************************
    Public functions.
*******************************************************************************/

extern  int  tcp_answer P_((TcpEndpoint listeningPoint,
			   double timeout,
			   TcpEndpoint *dataPoint)) ;

extern  int  tcp_call P_((const char *serverName,
			 int noWait,
             int lport,
			 TcpEndpoint *dataPoint)) ;

extern  int  tcp_complete P_((TcpEndpoint dataPoint,
			     double timeout,
			     int destroyOnError)) ;

extern  int  tcp_destroy P_((TcpEndpoint port)) ;

extern  int  tcp_fd P_((TcpEndpoint endpoint)) ;

extern  int  tcp_isreadable P_((TcpEndpoint dataPoint)) ;

extern  int  tcp_isup P_((TcpEndpoint dataPoint)) ;

extern  int  tcp_iswriteable P_((TcpEndpoint dataPoint)) ;

extern  int  tcp_listen P_((const char *portName,
			   int backlog,
			   TcpEndpoint *listeningPoint)) ;

extern  char  *tcp_name P_((TcpEndpoint endpoint)) ;

extern  int  tcp_read P_((TcpEndpoint dataPoint,
			 double timeout,
			 int numBytesToRead,
			 char *buffer,
			 int *numBytesRead)) ;

extern  int  tcp_request_pending P_((TcpEndpoint listeningPoint)) ;

#define  tcp_setbuf(endpoint, receiveSize, sendSize) \
    skt_setbuf(tcp_name(endpoint), tcp_fd(endpoint), receiveSize, sendSize)

extern  int  tcp_write P_((TcpEndpoint dataPoint,
			  double timeout,
			  int numBytesToWrite,
			  const char *buffer,
			  int *numBytesWritten)) ;


#ifdef __cplusplus
    }
#endif

#endif                          /* If this file was not INCLUDE'd previously. */
