
/*---------------------------------------------------------------------------*/
/*  Copyright (c) 1996 by Charles A. Measday                                 */
/*                                                                           */
/*  Permission to use, copy, modify, and distribute this software and its    */
/*  documentation for any purpose and without fee is hereby granted,         */
/*  provided that the above copyright notice appears in all copies. The      */
/*  author makes no representations about the suitability of this software   */
/*  for any purpose. It is provided "as is" without express or implied       */
/*  warranty.                                                                */
/*---------------------------------------------------------------------------*/

#include <unistd.h>
#include "net_priv.h"

/*----------------------------------------------------------------------*/

/**/

unsigned  long  net_addrof(

#    if __STDC__
        const  char  *hostName)
#    else
        hostName)

        char  *hostName ;
#    endif

{    /* Local variables. */
/*
 * Function: net_addrof.c
 * Version : 1.0
 * Created : 12-Jul-1996
 * Author  : Charles Measday
 *
 * Description
 *
 *    Function net_addrof() looks up a host by name and returns its IP address.
 *
 *    Invocation:
 *
 *        ipAddress = net_addrof(hostName);
 *
 *    where
 *
 *        <hostName>          - I
 *            Is the host's name. If this argument is NULL, the IP address
 *            of the local host is returned.
 *
 *        <ipAddress>         - O
 *            Returns the IP adddress of the host in network-byte order; zero
 *            is returned in the case of an error.
 *
 * Modification History
 *
 * Variables Used
 */

    char  localHost[MAXHOSTNAMELEN+1];
    unsigned  long  ipAddress;

/*
 * Main part of function.
 */

    /* If no host name was specified, use the local machine's host name.*/

    if (hostName == NULL) {

        if (gethostname(localHost, sizeof localHost)) {

            vperror("(netAddrOf) Error getting local host name.\ngethostname: ");
            return(0);

        }

        hostName = localHost;

    }

    /* If the host name was specified using the internet dot notation,  */
    /* then convert it to a binary address.                             */

    ipAddress = inet_addr(hostName);
    if (ipAddress != (unsigned long) -1)  return(ipAddress);

    /* Otherwise, an actual name was specified.  Look up the name in    */
    /* the operating system's host database.                            */

#ifdef VXWORKS
    if ((ipAddress = hostGetByName((char *) hostName)) == (unsigned long) ERROR) {

        vperror("(netAddrOf) Error getting host address for \"%s\".\nhostGetByName: ",
                hostName);
        return(0);

    }
#else
  { struct  hostent  *hostEntry ;

    if ((hostEntry = gethostbyname(hostName)) == NULL) {

        vperror("(netAddrOf) Error getting host entry for \"%s\".\ngethostbyname: ",
                hostName);
        return(0);

    }

    /* Try to avoid big-endian/little-endian problems by grabbing       */
    /* address as an unsigned long value.                               */

    if (hostEntry->h_length != sizeof(unsigned long)) {

        vperror("(netAddrOf) %s's address length is %d bytes, not %d bytes.\n",
                hostName, hostEntry->h_length, sizeof (unsigned long));
        return(0);

    }

    ipAddress = *((unsigned long *)hostEntry->h_addr);

  }
#endif

    return(ipAddress);

}

