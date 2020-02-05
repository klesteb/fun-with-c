
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

int  opt_init (

#    if __STDC__
        int  argc,
        char  *argv[],
        char *optionString,
        OptContext *scan)
#    else
        argc, argv, optionString, scan)

        int  argc ;
        char  *argv[] ;
        char *optionString;
        OptContext *scan;
#    endif

{
/*
 * Function: opt_init.c
 * Version : 1.0
 * Created : 12-Jul-1996
 * Author  : Charles Measday
 *
 * Description
 *
 *    Function opt_init() initializes a command line scan. A context structure,
 *    created and initialized by opt_init(), will keep track of the progress
 *    of the scan.  A (VOID *) pointer to the context structure is returned
 *    to the calling routine for use in subsequent opt_get() calls.
 *
 *    Invocation (with a single option string):
 *
 *        char  *optionString = "..." ;
 *        int  isList = 0 ;
 *        status = opt_init(argc, argv, isList, optionString, &scan);
 *
 *    Invocation (with an option string array):
 *
 *        char  *optionList[]  = { "...", "...", ..., NULL } ;
 *        int  isList = 1 ;
 *        status = opt_init(argc, argv, isList, optionList, &scan);
 *
 *    where:
 *
 *        <argc>              - I
 *            specifies the number of arguments in the command line's ARGV array.
 *
 *        <argv>              - I
 *            is an array of (CHAR *) pointers to the command line's arguments.
 *
 *        <optionString>      - I
 *            specify the possible command line options.  
 *            Within an option string, Options are specified as followed:
 *                "a"         Single-letter option
 *                "b:"        Single-letter option expecting an argument
 *                "{flag}"    Name option
 *                "{list:}"   Name option expecting an argument
 *
 *        <scan>              - O
 *            returns a handle for this command line scan's context.  If this
 *            argument is NULL, then a default context is used.
 *
 * Modification History
 *
 * Variables Used
 */

    char  *name, *s;
    int  i, length, numLetters, position;

/*
 * Main part of function.
 */

    /* Allocate a scan context block for the new options scan. If the   */
    /* caller does not want the context returned, then save it as the   */
    /* default context.                                                 */

    if (scan == NULL) {

        if (defaultContext != NULL)  opt_term(defaultContext);
#ifdef VXWORKS
        if (taskVarAdd(0, (int *)&defaultContext) == ERROR) {

            vperror("(opt_init) Error declaring the default context as a task variable.\ntaskVarAdd: ");
            return(errno);

        }
#endif
        scan = &defaultContext;

    } else {

	    if ((*scan = (OptContext)malloc(sizeof(_OptContext))) == NULL) {

    	    vperror("(opt_init) Error allocating scan context block.\nmalloc: ");
        	return(errno);

	    }

	}
	
    (*scan)->letterOptions = NULL;
    (*scan)->numNames = 0;
    (*scan)->nameOptions = NULL;
    (*scan)->printErrors = 1;
    (*scan)->argc = argc;
    (*scan)->argv = argv;
    (*scan)->arglen = NULL;
    (*scan)->endOfOptions = argc + 1;
    (*scan)->current = 1;
    (*scan)->offset = 0;
    (*scan)->optionType = none;
    (*scan)->nameString = NULL;

    /* Now, scan the option string(s) and count the number of           */
    /* single-letter options and the number of name options.            */

    numLetters = 0;  
    (*scan)->numNames = 0;

    s = optionString;

    while (*s != '\0') {                    /* Scan the option string.  */

        if (*s == '{') {                    /* Name option?             */

            (*scan)->numNames++;
            name = s;
            s = strchr(s, '}');

            if (s == NULL) {

                s = name + strlen(name);    /* Skip to end of string.   */

            } else s++;                     /* Advance past "}".        */

        } else {                            /* Single-letter option?    */
                                            /* Count letters.           */
            if (*s++ != ':')  numLetters++;

        }

    }     /* For each character in the option string */

    /* Allocate a string to hold the single-letter options and an array */
    /* to hold the index of each option. The index array is used to     */
    /* remember each option's position in the original options string.  */

    (*scan)->letterOptions = (char *)malloc(numLetters+1);

    if ((*scan)->letterOptions == NULL) {

        vperror("(opt_init) Error allocating string to hold %d single-letter options.\nmalloc: ",
                numLetters);
        return(errno);

    }

    (*scan)->letterOptions[numLetters] = '\0';

    (*scan)->letterIndex = (int *)malloc(numLetters * sizeof (int));
    if ((*scan)->letterIndex == NULL) {

        vperror("(opt_init) Error allocating array to hold %d single-letter indices.\nmalloc: ",
                 numLetters);
        return(errno);

    }

    /* Allocate an array to hold the name options and an array to hold  */
    /* the index of each option. The index array is used to remember    */
    /* each option's position in the original options string.           */

    (*scan)->nameOptions = (char **)malloc((*scan)->numNames * sizeof(char *)); 
    if ((*scan)->nameOptions == NULL) {

        vperror("(opt_init) Error allocating array to hold %d name options.\nmalloc: ",
                (*scan)->numNames);
        return(errno);

    }

    (*scan)->nameIndex = (int *)malloc((*scan)->numNames * sizeof (int));
    if ((*scan)->nameIndex == NULL) {

        vperror("(opt_init) Error allocating array to hold %d name indices.\nmalloc: ",
                (*scan)->numNames);
        return(errno);

    }

    /* Finally, scan the option strings again, copying single-letter    */
    /* options and name options to the appropriate fields in the context*/
    /* structure.                                                       */

    numLetters = 0;
    (*scan)->numNames = 0;  
    position = 1;

    s = optionString;

    while (*s != '\0') {                    /* Scan the option string. */

        if (*s == '{') {                    /* Name option? */

            name = s + 1;
            s = strchr (s, '}');
            length = (s == NULL) ? strlen (name) : (s - name);

            if (name[length-1] == ':') {

                length--;
                (*scan)->nameIndex[(*scan)->numNames] = -position++;

            } else {

                (*scan)->nameIndex[(*scan)->numNames] = position++;

            }
                                                /* Wastefully duplicate name. */
            (*scan)->nameOptions[(*scan)->numNames] = strdup(name);
            (*scan)->nameOptions[(*scan)->numNames++][length] = '\0';

            if (s == NULL) {

                 s = name + strlen (name);   /* Skip to end of string. */

            } else s++;                    /* Advance past "}".    */

        } else {                            /* Single-letter option?*/
                                                /* Copy letters.        */
            (*scan)->letterOptions[numLetters] = *(s++);

            if (*s == ':') {

                (*scan)->letterIndex[numLetters] = -(position++);
                s++;

            } else {

                (*scan)->letterIndex[numLetters] = position++;

            }

            numLetters++;

        }

    }     /* For each character in the option string */

    /* If there are name options, then call opt_reset() to construct a  */
    /* table containing the length of each argument in the ARGV array.  */
    /* Doing so now will save opt_get() the trouble of doing so when    */
    /* trying to match abbreviated name options.                        */

    if (opt_reset(*scan, argc, argv)) {

        vperror("(opt_init) Error building table of argument lengths.\nopt_reset: ");
        return(errno);

    }


    if (opt_util_debug) {

        printf("(opt_init) Single-letter options = \"%s\"\n",
                (*scan)->letterOptions);

        for (i = 0; (*scan)->letterOptions[i] != '\0';  i++) {

            printf("(opt_init) Letter option (%d) = '%c'\n",
                   (*scan)->letterIndex[i], (*scan)->letterOptions[i]);

        }

        for (i = 0;  i < (*scan)->numNames;  i++) {

            printf("(opt_init) Name option (%d) = \"%s\"\n",
                   (*scan)->nameIndex[i], (*scan)->nameOptions[i]);

        }

    }

    return(0);

}

