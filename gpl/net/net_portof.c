
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

int  net_portof(

#    if __STDC__
        const  char  *serverName,
        const  char  *protocol)
#    else
        serverName, protocol)

        char  *serverName ;
        char  *protocol ;
#    endif

{    /* Local variables. */
/*
 * Function: net_portof.c
 * Version : 1.0
 * Created : 12-Jul-1996
 * Author  : Charles Measday
 *
 * Description
 *
 *    Function net_portof() looks up a server's name in the network services
 *    database (the "/etc/services" file) and returns the server's port
 *    number.
 *
 *    NOTE:  net_portof() makes use of the system getservbyname(3) function
 *    under operating system's that support that function.  Otherwise, it
 *    manually reads the "/etc/services" file in search of the server name.
 *    In this latter case, a file other than "/etc/services" can be used as
 *    the services database by storing the full pathname of the file in
 *    environment variable, "SERVICES_FILE".
 *
 *    Invocation:
 *
 *        port = net_portof(serverName, protocol);
 *
 *    where
 *
 *        <serverName>        - I
 *            Is the name of the service being looked up.
 *
 *        <protocol>          - I
 *            Specifies a network protocol; e.g., "tcp" or "udp".  A given
 *            service name may appear under different protocols in the
 *            services file.  If PROTOCOL is specified, then net_portof()
 *            returns the server's port for that protocol.  If PROTOCOL is
 *            NULL, then net_portof() returns the first port for the service
 *            name, regardless of the protocol.
 *
 *        <port>              - O
 *            Returns the requested service's port number in host byte order;
 *            -1 is returned in the event of an error (e.g., the service was
 *            not found).
 *
 * Modification History
 *
 * Variables Used
 */

    char  *s ;
    int  portNumber ;
#ifdef NO_GETSERVBYNAME
    char  *fileName, inbuf[128] ;
    char  *s_name, *s_port, *s_protocol ;
    FILE  *infile ;
#    ifdef VXWORKS                      /* Use reentrant version of STRTOK(). */
#        define  strtok(string, separators) \
                    strtok_r (string, separators, &last)
        char  *last ;
#    endif
#else
    struct  servent  *serverEntry ;
#endif

/*
 * Main part of function.
 */

    /* If the server "name" is simply the desired port number in ASCII, */
    /* then convert and return the binary port number to the caller.    */

    portNumber = strtol(serverName, &s, 0);
    if (*s == '\0') {

        if (portNumber < 0) {

            errno = EINVAL;
            vperror("(netPortOf) Invalid port number: %d\n", portNumber);

        }

        return(portNumber);

    }

#ifndef NO_GETSERVBYNAME

    /* Under operating systems that support getservbyname(3), call it.  */

    if ((serverEntry = getservbyname(serverName, NULL)) == NULL) {

        if (errno == 0)  errno = EINVAL;

        vperror("(net_portof) Error getting server entry for %s.\ngetservbyname: ",
                serverName);
        return(-1);

    }

    return(ntohs(serverEntry->s_port));

#else


    /* Open the network services file.  The name of the file is         */
    /*  retrieved from environment variable "SERVICES_FILE".  If that   */
    /* variable has no translation, then the file name defaults to the  */
    /* "/etc/services" system file.                                     */

    fileName = getenv("SERVICES_FILE");
    if (fileName == NULL)  fileName = "/etc/services";

    if ((infile = fopen(fileName, "r")) == NULL) {

        vperror("(net_portof) Error opening \"%s\" file.\nfopen: ",
                fileName);
        return(-1);

    }

    /* Scan the network services file for the requested service name.   */

    for (;;) {

        /* Read the next server entry from the file. Blank lines and    */
        /* comments are skipped.                                        */

        if (fgets(inbuf, (sizeof inbuf), infile) == NULL) {

            if (ferror(infile))
                vperror("(net_portof) Error reading %s.\nfgets: ", fileName);

            fclose(infile);
            return(-1);

        }

        /* Strip comments and form feeds.                               */

        if ((s = strchr(inbuf, '#')) != NULL)  *s = '\0';

        while ((s = strchr(inbuf, '\f')) != NULL)  *s = ' ';

        /* Extract the service name, port number, and protocol from the */
        /* entry.                                                       */
                                                
#ifdef VXWORKS
        last = NULL ;                           /* For STRTOK_R(2).     */
#endif
        s_name = strtok(inbuf, " \t\n/");       /* Service name.        */
        if (s_name == NULL) continue;           /* Skip blank lines.    */
        s_port = strtok(NULL, " \t\n/");        /* Port number.         */
        if (s_port == NULL) continue;           /* No port number?      */
        s_protocol = strtok(NULL, " \t\n/");    /* Protocol.            */
        if (s_protocol == NULL) continue;       /* No protocol?         */

        /* If the protocols match, then scan the name and the aliases   */
        /* for the desired service name.                                */

        if ((protocol != NULL) && (strcmp(protocol, s_protocol) != 0))
            continue;                          /* Protocols don't match?*/

        while (s_name != NULL) {               /* Scan name and aliases.*/

            if (strcmp(s_name, serverName) == 0) break;
            s_name = strtok(NULL, " \t\n/");

        }

        if (s_name != NULL) break;             /* Desired service found?*/

    }

    /* An entry for the desired service was found.  Close the services  */
    /* file and return the service's port number to the calling routine.*/

    fclose(infile);

    return(atoi(s_port));

#endif    /* NO_GETSERVBYNAME */

}

