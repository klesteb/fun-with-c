
/*---------------------------------------------------------------------------*/
/*                                                                           */
/* TITLE:       tcp_priv.h                                                   */
/*                                                                           */
/* FACILITY:    Private variables and structures for the TCP package.        */
/*                                                                           */
/* ABSTRACT:    These are the declarations of the data structures and        */
/*              routines to implement the TCP utilities.                     */
/*                                                                           */
/* ENVIRONMENT: VMS/AIX/Linux                                                */
/*                                                                           */
/* Version      Author                                           Date        */
/* -------      ---------------------------------------------    ----------- */
/* 1-000        Kevin Esteb                                      23-Apr-2002 */
/*                                                                           */
/*---------------------------------------------------------------------------*/

#ifndef TCP_PRIV_H
#define TCP_PRIV_H 1

#define  _BSD_SOURCE  1

#include  <ctype.h>                     /* Standard character functions. */
#include  <errno.h>                     /* System error definitions. */
#include  <limits.h>                    /* Maximum/minimum value definitions. */
#ifndef PATH_MAX
#    include  <sys/param.h>             /* System parameters. */
#    define  PATH_MAX  MAXPATHLEN
#endif
#include  <stdio.h>                     /* Standard I/O definitions. */
#include  <stdlib.h>                    /* Standard C Library definitions. */
#include  <string.h>                    /* C Library string functions. */

#if defined(VMS)
#    include  <socket.h>                /* Socket-related definitions. */
#    include  <ucx$inetdef.h>           /* VMS/Ultrix Connection definitions. */
#    include  <unixio.h>                /* VMS-emulation of UNIX I/O. */
#    include  <ioctl.h>                 /* UNIX ioctl() socket definition. */
#    include  "fd.h"                    /* File descriptor set definitions. */
#    define  MAXHOSTNAMELEN  64
#elif defined(VXWORKS)
#    include  <hostLib.h>               /* Host library definitions. */
#    include  <ioLib.h>                 /* I/O library definitions. */
#    include  <selectLib.h>             /* SELECT(2) definitions. */
#    include  <socket.h>                /* Socket-related definitions. */
#    include  <sockLib.h>               /* Socket library definitions. */
#    include  <sysLib.h>                /* System library definitions. */
#    include  <taskLib.h>               /* Task library definitions. */
#    define  sleep(seconds)  taskDelay ((seconds) * sysClkRateGet())
#    include  <types.h>                 /* System type definitions. */
#    include  <unistd.h>                /* UNIX I/O definitions. */
#    include  <sys/ioctl.h>             /* I/O control definitions. */
#    include  <sys/times.h>             /* System time definitions. */
#else
#    include  <netdb.h>                 /* Network database definitions. */
#    include  <unistd.h>                /* UNIX I/O definitions. */
#    include  <sys/ioctl.h>             /* I/O control definitions. */
#    include  <sys/param.h>             /* System parameters. */
#    include  <sys/socket.h>            /* Socket-related definitions. */
#    include  <sys/time.h>              /* System time definitions. */
#    include  <sys/types.h>             /* System type definitions. */
#endif

#include  <netinet/in.h>                /* Internet IPC domain definitions. */

#include  "meo_util.h"                  /* Memory operations. */
#include  "net_util.h"                  /* Networking utilities. */
#include  "skt_util.h"                  /* Socket support functions. */
#include  "str_util.h"                  /* String manipulation functions. */
#include  "tv_util.h"                   /* "timeval" manipulation functions. */
#include  "vperror.h"                   /* VPERROR() definitions. */
#include  "tcp_util.h"                  /* TCP/IP networking utilities. */

                                        /* Delete an endpoint without
                                           overwriting the value of ERRNO. */
#define  CLEAN_UP(endpoint)     \
      { int  status = errno ;   \
        tcp_destroy (endpoint) ; \
        endpoint = NULL ;       \
        errno = status ;        \
      }

/*----------------------------------------------------------------------*/
/* TCP Endpoint - contains information about a server's listening       */
/* endpoint or about a client's or server's data endpoint.              */
/*----------------------------------------------------------------------*/

typedef  enum  TcpEndpointType {
    TcpNone, TcpListeningPoint, TcpDataPoint
}  TcpEndpointType ;

typedef  struct  _TcpEndpoint {
    char  *name ;                       /* "<port>[@<host>]" */
    TcpEndpointType  type ;             /* Listening or data endpoint? */
    int  fd ;                           /* Listening or data socket. */
}  _TcpEndpoint ;

#endif

