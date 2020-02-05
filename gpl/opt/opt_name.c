
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

#include "opt_priv.h"

/*----------------------------------------------------------------------*/

/**/

char  *opt_name (

#    if __STDC__
        OptContext  scan,
        int  index)
#    else
        scan, index)

        OptContext  scan ;
        int  index ;
#    endif

{
/*
 * Function: opt_name.c
 * Version : 1.0
 * Created : 12-Jul-1996
 * Author  : Charles Measday
 *
 * Description
 *
 *    Function opt_name() returns the name corresponding to an option number
 *    returned by opt_get(). For the current option on the command line,
 *    opt_get() returns the position, 1..N, of that option within the
 *    set of options specified when opt_init() was called. opt_name()
 *    simply returns the textual version of the option. For example,
 *    if "d" was the third option in opt_init()'s options string, then
 *    opt_get() would return 3 if it encountered "-d" on the command
 *    line and opt_name(3) would return "-d".
 *
 *    Invocation:
 *
 *        name = opt_name(scan, index);
 *
 *    where:
 *
 *        <scan>              - I
 *            Is the command line scan context originally created by opt_init().
 *            If this argument is NULL, the default scan context is used.
 *
 *        <index>             - I
 *            Is the index (1..N) of the option; i.e., is this option the 1st,
 *            2nd, or Nth option in the set of options specified in the call
 *            to opt_init()?  NONOPT, OPTEND, and OPTERR can be specified.
 *
 *        <name>              - O
 *            Returns the name of the option corresponding to the specified
 *            index. Word options do not have the enclosing curly braces.
 *            Options expecting an argument have a trailing ':'.  "NONOPT:",
 *            "OPTEND", and "OPTERR:" are returned for NONOPT, OPTEND, and
 *            OPTERR, respectively. The text of the name is stored in memory
 *            belonging to the OPT utilities and it should not be modified by
 *            the calling routine. Furthermore, the name should be used or
 *            duplicated before calling opt_name() again.
 *
 * Modification History
 *
 * Variables Used
 */

    int  i, length;

/*
 * Main part of function.
 */

    if (scan == NULL) scan = defaultContext;
    if (scan == NULL) return("");

    /* Deallocate the previously-returned name string.                  */

    if (scan->nameString != NULL)  free(scan->nameString);
    scan->nameString = NULL;

    /* Determine the name corresponding to the specified option index.  */

    switch (index) {
        case NONOPT:
            return("NONOPT:");

        case OPTEND:
            return("OPTEND");

        case OPTERR:
            return("OPTERR:");

        default:                            /* Scan the letter options. */
            for (i = 0;  scan->letterOptions[i] != '\0';  i++) {

                if (scan->letterOptions[i] == ':')  continue;

                if ((scan->letterIndex[i] < 0) &&
                    (index == -scan->letterIndex[i])) {

                    scan->nameString = malloc(1 + 3);
                    sprintf(scan->nameString, "-%c:", scan->letterOptions[i]);

                    return(scan->nameString);

                } else if (index == scan->letterIndex[i]) {

                    scan->nameString = malloc(1 + 2);
                    sprintf(scan->nameString, "-%c", scan->letterOptions[i]);

                    return(scan->nameString);

            }

        }
                                        /* Scan the word options. */
        for (i = 0;  i < scan->numNames;  i++) {

            if ((scan->nameIndex[i] < 0) && (index == -scan->nameIndex[i])) {

                length = strlen(scan->nameOptions[i]);
                scan->nameString = malloc(length + 3);
                sprintf(scan->nameString, "-%s:", scan->nameOptions[i]);

                return(scan->nameString);

            } else if (index == scan->nameIndex[i]) {

                length = strlen(scan->nameOptions[i]);
                scan->nameString = malloc(length + 2);
                sprintf(scan->nameString, "-%s", scan->nameOptions[i]);

                return(scan->nameString);

            }

        }
                                        /* If not found in either, return "". */
        return("");

    }

}

