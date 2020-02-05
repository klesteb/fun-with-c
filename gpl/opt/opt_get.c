
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

int  opt_get (

#    if __STDC__
        OptContext  scan,
        char  **argument)
#    else
        scan, argument)

        OptContext  scan ;
        char  **argument ;
#    endif

{
/*
 * Function: opt_get.c
 * Version : 1.0
 * Created : 12-Jul-1996
 * Author  : Charles Measday
 *
 * Description
 *
 *    Function opt_get() gets the next option and its argument (if there is 
 *    one) from the command line.
 *
 *    Name options can be abbreviated on the command line.  For example,
 *    a "-flag" option could be entered on the command line as "-f", "-fl",
 *    "-fla", or "-flag".  If, however, the one-character abbreviation of
 *    a name option (e.g., "-f" for "-flag") conflicts with a single-letter
 *    option (e.g., "-f"), the single-letter option takes priority.
 *
 *    The following options receive special handling:
 *
 *        "-"     A hyphen by itself is returned to the calling routine as
 *                a non-option argument.
 *
 *        "--"    Two hyphens indicate the end of option processing; the
 *                remaining arguments on the command line are treated as
 *                non-option arguments.
 *
 *        "< input_file"
 *        "> output_file"
 *                These symbols cause UNIX shell-style I/O redirection.
 *
 *    Invocation:
 *
 *        index = opt_get(scan, &argument);
 *
 *    where:
 *
 *        <scan>          - I
 *            is the command line scan context created by opt_init().  If
 *            this argument is NULL, the default scan context is used.
 *
 *        <argument>      - O
 *            returns a (CHAR *) pointer to the next option's argument.  NULL
 *            is returned for single-letter options without arguments and
 *            name options without arguments.  If an invalid option is
 *            encountered and OPTERR is returned as the INDEX (see below),
 *            then ARGUMENT returns a pointer to the option in error.
 *
 *        <index>         - I
 *            returns the index (1..N) of the next option; i.e., is this option
 *            the 1st, 2nd, or Nth option in the set of options specified in
 *            the call to opt_init()?  For example, if "ab:{flag}c" are the
 *            allowable options, then the appearance of "-c" on the command
 *            line will be returned by opt_get() as an index of 4.  OPTEND
 *            (zero) is returned when the command line scan is complete;
 *            OPTERR is returned for invalid options and missing arguments.
 *
 * Modification History
 *
 * Variables Used
 */

    char  *group, *s;
    int  i, optionIndex = OPTERR;

/*
 * Main part of function.
 */

    *argument = NULL;
    if (scan == NULL)  scan = defaultContext;

    if (scan == NULL) {

        errno = EINVAL;
        vperror("(opt_get) Null scan context.\n");
        return(OPTERR);

    }

    /* The CURRENT and OFFSET values in the scan context index the last */
    /* option or argument returned by opt_get(). Depending on the type  */
    /* of the last option, increment these values to index the next     */
    /* option on the command line.                                      */

    switch (scan->optionType) {
        case none:
            /* Beginning of scan: position set by OPT_INIT() or         */
            /* opt_reset(). End of scan: don't advance.                 */
            break;

        case invalid_option:
            if ((scan->numNames > 0) && (scan->offset == 1)) {

                scan->current++;                   /* Skip invalid name option. */
                scan->offset = 0;
                break;

            } else {                                /* Skip invalid letter option. */

                /* Continue on into the "letter" case. */

            }

        case letter:
            scan->offset++;
            if (scan->argv[scan->current][scan->offset] == '\0') {

                scan->current++;
                scan->offset = 0;

            }
            break;

        case missing_argument:
            /* The option expecting an argument was either at the end   */
            /* of the command line (e.g., "command ... -b") or was      */
            /* followed by another options group (e.g., "command ...    */
            /* -b -c").  In the latter case, stay put; CURRENT is       */
            /* already set for the new options group.                   */
            scan->offset = 0;
            break;

        case io_redirection:
        case letter_with_argument:
        case name:
        case name_with_argument:
        case non_option_argument:
        default:
            scan->current++;  
            scan->offset = 0;
            break;

    }

    /* Scan the command line and return the next option or, if none,    */
    /* the next non-option argument.  At the start of each loop         */
    /* iteration, the CURRENT and OFFSET fields in the scan context     */
    /* structure index the next option/argument to be examined.         */

    for ( *argument = NULL;  scan->current < scan->argc;  *argument = NULL) {

        group = scan->argv[scan->current];

        /* If the current option group begins with '-', then check for  */
        /* the special hyphen cases ("-" and "--") and for single-letter*/
        /* or name options.                                             */

        if ((group[0] == '-') && (scan->current < scan->endOfOptions)) {

            /* Check for the special options, "-" and "--".  Return "-" */
            /* as a non-option argument.  For the "--" end-of-options   */
            /* indicator, remember the current position on the command  */
            /* line so that the remaining arguments are all treated as  */
            /* non-option arguments.                                    */

            if (scan->offset == 0) {

                if (group[1] == '\0') {                 /* "-" */

                    scan->optionType = non_option_argument;
                    *argument = group;
                    return(NONOPT);

                } else if ((group[1] == '-') &&         /* "--" */
                           (group[2] == '\0')) {

                    scan->endOfOptions = scan->current++;
                    continue;

                } else {                                /* "-xyz" */

                    scan->offset++;

                }

            } else if (group[scan->offset] == '\0') {

                /* Are we at the end of the current options group? If   */
                /* so, loop and try the next command line argument.     */

                scan->current++;
                scan->offset = 0;
                continue;

            }

            /* If we're at the beginning of the options group, check if */
            /* a name option was specified. Using strncmp()             */
            /* (length-restricted comparison) to match name options     */
            /* means that name options can be abbreviated. For example, */
            /* expected option "{name}" will be matched by any of the   */
            /* following on the command line: "-n", "-na", "-nam", or   */
            /* "-name".                                                 */

            scan->optionType = none;

            if (scan->offset == 1) {
                                                /* Scan the name list. */
                for (i = 0;  i < scan->numNames;  i++) {

                    if (strncmp(&group[1], scan->nameOptions[i],
                                scan->arglen[scan->current] - 1) == 0) {
                        break;

                    }

                }

                /* Found and no conflict with single-letter option?     */

                if ((i < scan->numNames) &&
                    ((scan->arglen[scan->current] > 2) ||
                     (strchr (scan->letterOptions, group[1]) == NULL))) {

                    optionIndex = scan->nameIndex[i];

                    if (optionIndex < 0) {

                        scan->optionType = name_with_argument;
                        optionIndex = -optionIndex;

                    } else {

                        scan->optionType = name;

                    }

                }

            }

            /* Otherwise, check for a single-letter option.             */

            if ((scan->optionType == none) &&
                (s = strchr(scan->letterOptions, group[scan->offset]))) {

                i = s - scan->letterOptions;
                optionIndex = scan->letterIndex[i];

                if (optionIndex < 0) {

                    scan->optionType = letter_with_argument;
                    optionIndex = -optionIndex;

                } else {

                    scan->optionType = letter;

                }

            }

            /* If the current option is not one of the allowed          */
            /* single-letter or name options, then signal an error.     */

            if (scan->optionType == none) {

                if (scan->printErrors) {

                    if ((scan->numNames > 0) && (scan->offset == 1)) {

                        fprintf(stderr, "%s: illegal option -- %s\n",
                                scan->argv[0], &group[scan->offset]);

                    } else {

                        fprintf(stderr, "%s: illegal option -- %c\n",
                                scan->argv[0], group[scan->offset]);

                    }

                }

                scan->optionType = invalid_option;
                *argument = &group[scan->offset];
                return(OPTERR);

            }

        } else {     /* If the option group begins with a hyphen ('-')  */

            /* The current options group does NOT begin with a hyphen.  */
            /* Check for I/O redirection or a non-option argument.      */
            /* Check for I/O redirection, indicated by "<" (input) or   */
            /* ">" (output) characters.                                 */

            if ((scan == defaultContext) &&
                ((group[0] == '<') || (group[0] == '>'))) {

                scan->optionType = io_redirection;

            } else {

                /* This is a non-option argument. Return the argument   */
                /* to the calling routine.                              */

                scan->optionType = non_option_argument;
                *argument = group;     /* Return NONOPT and argument.   */
                return(NONOPT);

            }

        }     /* If the option group does NOT begin with a hyphen ('-') */

        /* This point in the code is reached if a single-letter or name */
        /* option was specified, or if I/O redirection was requested.   */
        /*                                                              */
        /* If an argument is expected (because the option expects one   */
        /* or if I/O is being redirected to a file), then get the       */
        /* argument. For single-letter options and I/O redirection, the */
        /* argument may be flush up against the option (i.e., the       */
        /* argument is the remainder of the current ARGV) or it may be  */
        /* separated from the option by white space (i.e., the argument */
        /* is the whole of the next ARGV). For name options, the        */
        /* argument is always found in the next ARGV.                   */

        switch (scan->optionType) {
            case io_redirection:
            case letter_with_argument:
                scan->offset++;
                if (group[scan->offset] == '\0') {          /* Argument separated by space? */

                    scan->current++;

                    if ((scan->current >= scan->argc) ||    /* Missing argument? */
                        (*scan->argv[scan->current] == '-')) {

                        if (scan->printErrors) {

                            fprintf(stderr, "%s: option requires an argument -- %c\n",
                                    scan->argv[0], group[scan->offset-1]);

                        }

                        scan->optionType = missing_argument;
                        *argument = &group[scan->offset-1];
                        return(OPTERR);

                    }

                    scan->offset = 0;

                }

                *argument = &scan->argv[scan->current][scan->offset];
                break;

            case name_with_argument:
                scan->current++;
                if ((scan->current >= scan->argc) ||        /* Missing argument? */
                    (*scan->argv[scan->current] == '-')) {

                    if (scan->printErrors) {

                        fprintf(stderr, "%s: option requires an argument -- %s\n",
                                scan->argv[0], &group[1]);

                    }

                    scan->optionType = missing_argument;
                    *argument = &group[1];
                    return(OPTERR);

                }

                *argument = scan->argv[scan->current];
                break;

            default:
                *argument = NULL;
                break;

        }

        /* If I/O redirection was requested, then redirect standard     */
        /* input or standard output to the specified file.              */

        if (scan->optionType == io_redirection) {

            switch (group[0]) {
                case '<':
                    if (freopen (*argument, "r", stdin) == NULL) {

                        if (scan->printErrors) {

                            fprintf(stderr, "%s: unable to redirect input from %s\n",
                                    scan->argv[0], *argument);
                            perror("freopen");

                        }

                        return(OPTERR);

                    }
                    break;

                case '>':
                    if (freopen (*argument, "w", stdout) == NULL) {

                        if (scan->printErrors) {

                            fprintf(stderr, "%s: unable to redirect output to %s\n",
                                    scan->argv[0], *argument);
                            perror("freopen");

                        }

                        return(OPTERR);

                    }
                    break;

                default:
                    break;

            }

            scan->current++;                   /* Position to next argument. */
            scan->offset = 0;
            continue;                          /* Loop for next option. */

        }

        /* At last! A valid option and (optionally) its argument have   */
        /* been located. Return it (or them) to the calling routine.    */

        return(optionIndex);

    }     /* For each argument on the command line                      */

    /* The end of the command line has been reached. Signal the calling */
    /* routine that there are no more arguments.                        */

    return(OPTEND);

}

