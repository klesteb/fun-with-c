
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

#include "net_priv.h"

/*----------------------------------------------------------------------*/

/**/

char  *net_hostof(

#    if __STDC__
        unsigned  long  ipAddress)
#    else
        ipAddress)

        unsigned  long  ipAddress ;
#    endif

{    /* Local variables. */
/*
 * Function: net_hostof.c
 * Version : 1.0
 * Created : 12-Jul-1996
 * Author  : Charles Measday
 *
 * Description
 *
 *    Function net_hostof() looks up an IP address and returns the corresponding
 *    host name.
 *
 *    Invocation:
 *
 *        hostName = net_hostof(ipAddress);
 *
 *    where
 *
 *        <ipAddress>         - I
 *            Is the IP adddress of the host.
 *
 *        <hostName>          - O
 *            Returns the host's name; NULL is returned in the event of an
 *            error. The ASCII name string is stored local to net_hostof()
 *            and it should be used or duplicated before calling net_hostof()
 *            again.
 *
  * Modification History
 *
 * Variables Used
 */

    char  dottedName[MAXHOSTNAMELEN+1], *hostName ;
    struct  in_addr  address ;
#if CACHE_NAMES
    static  HashTable  cachedNames = NULL ;
#endif

/*
 * Main part of function.
 */

    /* On the first call to net_hostof(), create the table of cached    */
    /* host names.                                                      */

#if CACHE_NAMES
    if ((cachedNames == NULL) && hash_create(64, 0, &cachedNames)) {

        vperror("(net_hostof) Error creating host name cache.\nhashCreate: ");
        return(NULL);

    }
#endif

    /* Convert the IP address to its dotted format, "a.b.c.d".          */

    address.s_addr = ipAddress;
#ifdef VXWORKS
    inet_ntoa_b(address, dottedName);
#else
    strcpy(dottedName, inet_ntoa(address));
#endif

    /* To avoid having to query the name server, look up the dotted IP  */
    /* address in the host name cache.                                  */

#if CACHE_NAMES
    if (hash_search(cachedNames, dottedName, (void **)&hostName))
        return(hostName);
#endif

    /* Bummer!  The name server must be queried for the host's name.    */

#ifdef VXWORKS
    if (hostGetByAddr(ipAddress, dottedName)) {

        inet_ntoa_b(address, dottedName);       /* Restore IP address.  */
        hostName = strdup(dottedName);          /* ASCII IP address.    */

    } else {

        hostName = strdup(dottedName);          /* Actual host name.    */
        inet_ntoa_b(address, dottedName);       /* Restore IP address.  */

    }
#else
  { struct  hostent  *host;

    host = gethostbyaddr((const char *) &address, sizeof address, AF_INET);
    if (host == NULL) {

        hostName = strdup(dottedName);          /* ASCII IP address.    */

    } else {

        hostName = strdup(host->h_name);        /* Actual host name.    */

    }

  }
#endif

    if (hostName == NULL) {

        vperror("(netHostOf) Error duplicating host name.\nstrdup: ");
        return(NULL);

    }

    /* Cache the new translation for future reference.                  */

#if CACHE_NAMES
    if (hash_add(cachedNames, dottedName, (void *)hostName)) {

        vperror("(netHostOf) Error caching host name.\nhashAdd: ");

    }
#endif

    return(hostName);

}

