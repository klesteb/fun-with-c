/************************************************************************
 *  Copyright (c) 1996 by Charles A. Measday                            *
 *                                                                      *
 *  Permission to use, copy, modify, and distribute this software       *
 *  and its documentation for any purpose and without fee is hereby     *
 *  granted, provided that the above copyright notice appear in all     *
 *  copies.  The author makes no representations about the suitability  *
 *  of this software for any purpose.  It is provided "as is" without   *
 *  express or implied warranty.                                        *
 ************************************************************************/

/*
@(#)  FILE: rex_util.c  RELEASE: 1.15  DATE: 11/25/98, 19:11:56
*/
/*******************************************************************************

File:

    rex_util.c

    Regular Expression Matching Utilities


Author:    Alex Measday, ISI


Purpose:

    These utilities provide a simple, regular expression (RE) matching
    capability.  These utilities are similar to the REGCMP(3) utilities
    (from the System V Programmers Workbench), but REGCMP(3) isn't
    available on all systems.

    The REX utilities support the following regular expression constructs:

        Basic RE symbols:     . ^ $
        Character classes:    [] ^ -
        [:class:]    match or ...
        [^:class:]   ... doesn't match a character in the specified class,
                     where the possible classes are "alpha", "upper", "lower",
                     "digit", "xdigit", "alnum", "space", "punct", "print",
                     "cntrl", and "graph" (see CTYPE(3) for descriptions).
        RE*          matches zero or more instances of the RE.
        RE+          matches one or more instances of the RE.
        RE?          matches zero or one instance of the RE.
        RE1RE2       matches RE1 followed immediately by RE2 (no intervening
                     spaces in the RE pattern or in the target string).
        RE1|RE2      matches RE1 or RE2.
        (RE)         parentheses allow grouping of RE's.
        (RE)$n       returns a pointer to the text matched by the RE in the
                     N-th return argument.  N is a single digit between zero
                     and 9, inclusive.
        RE{[m][,[n]]}
                     matches M through N instances of the RE.  If not
                     specified, M defaults to 0.  Depending on whether or
                     not the comma is present, N defaults to M ("RE{m}")
                     or a very large number ("RE{m,}").  "RE*" is equivalent
                     to "RE{0,}".  "RE+" is equivalent to "RE{1,}".  "RE?" is
                     equivalent to "RE{0,1}".

    The REGCMP(3) utilities support everything except alternation ("RE1|RE2"),
    zero-or-one closure ("RE?"), and character classification matching
    ("[:class:]").  Its "RE{m,n}" construct limits M and N to 256.

    The REX_UTIL package's text substitution function, REX_REPLACE, supports
    the following special character sequences in replacement text:

        $0 - $9    Insert subexpression (0..9) matched by RE.
        $&         Insert entire text matched by RE.
        $l0 - $l9  Insert subexpression (0..9) matched by RE, converted to
                   lower case.
        $l&        Insert entire text matched by RE, converted to lower case.
        $u0 - $u9  Insert subexpression (0..9) matched by RE, converted to
                   upper case.
        $u&        Insert entire text matched by RE, converted to upper case.
        \c         Insert character 'c' (e.g., "\$" gives "$").

    The YACC grammar used to parse the regular expressions is based on the
    grammar found in Chapter 21 ("Parsing") of Robert Sedgewick's book,
    ALGORITHMS.  The graph representation of a regular expression is based
    on the representation described in Chapter 20 ("Pattern Matching").
    The actual matching of a regular expression to a string is performed
    using a fairly straightforward, recursive search of the RE graph - I
    never quite got a handle on Sedgewick's use of double-ended queues!
    HP/UX's REGEXP(5) documentation was used as the authoritative source
    on syntax and semantics of regular expressions.  The idea of generating
    the set of first characters in a regular expression was "stolen" from
    GNU's REGEX functions (they called it a "fastmap").  Henry Spencer's
    test set of regular expressions was used to check out the REX utilities
    (more on that below).

    The REX utilities are slower than the UNIX regular expression utilities.
    The REGEXP(3) routines are very fast, but they only support a very basic
    set of regular expression constructs.  The REGCMP(3) routines aren't
    quite as fast, but they support a more powerful set of regular expression
    constructs.  The REX utilities come in third, but their performance is
    not bad.  100,000 matches of "aaabbbcdef" by RE "a*b*cdef" on a Sparc1
    workstation took 12 seconds for the REX utilities (that's about 8300
    matches per second) and only 2 seconds for the REGEXP(3) utilities.
    Tests of the REGCMP(3) utilities on an HP workstation showed that they
    are about 4 times as fast as the REX utilities.

    I obtained a copy of UNIX V8-compatible REGEXP(3) routines (not to
    be confused with the REGEXP(3) routines mentioned above) from UUNET.
    These functions were written by Henry Spencer, a noted UNIX guru at
    the University of Toronto.  The REX utilities were generally faster
    than these REGEXP(3) routines, but used more memory; the REGEXP(3)
    routines used a flat character string representation for REs inspired
    by Kernighan and Plauger's approach in their book, SOFTWARE TOOLS.

    I used the test set supplied with Henry Spencer's REGEXP(3) code
    to work the kinks out of the REX utilities.  The major change that
    resulted from this exercise was the check for nested, empty closure
    expressions while parsing an RE.  Expressions such as "(a*)*" are
    now rejected, since they would cause endless cycling during a
    pattern match; innocuous nestings like "(a*)?" are also rejected.
    Null expressions (e.g., "()" and "(abc|)def") are now allowed.
    Also, "^" and "$" are always treated as beginning- and end-of-line
    anchors, unless they're escaped by a forward slash.  Consequently,
    regular expressions such as "^abc|^def$" are now possible.


Public Procedures:

    REX_COMPILE - compiles a regular expression.

    REX_DELETE - deletes a compiled regular expression.

    REX_DUMP - dumps a compiled regular expression pattern to a file.

    REX_MATCH - attempts to match a subject string against a compiled RE.

    REX_NOSEX - returns the number of subexpression assignments defined for
        a compiled RE.

    REX_REPLACE - search for and replace text matched by a compiled RE.

    REX_WILD - converts a CSH(1)-style wildcard expression to an RE.

Private Procedures:

    REX_DUMP_CLASS - dumps a character class to a file.

    REX_SEARCH - performs a graph search of the RE in order to match the RE
        to the target string.

Public Variables:

    REX_ERROR_TEXT - is a (CHAR *) pointer to a short error message
        explaining why REX_COMPILE failed, if it does.

*******************************************************************************/

/*#define  _SVID_SOURCE  1*/

#include  <ctype.h>			/* Standard character functions. */
#include  <errno.h>			/* System error definitions. */
#if __STDC__
#    include  <stdarg.h>		/* Variable-length argument lists. */
#else
#    include  <varargs.h>		/* Variable-length argument lists. */
#endif
#include  <stdio.h>			/* Standard I/O definitions. */
#include  <stdlib.h>			/* Standard C Library definitions. */
#include  <string.h>			/* Standard C string functions. */
#ifndef isascii
#    define  isascii(c)  ((unsigned char) (c) <= 0177)
#endif
#include  "str_util.h"			/* String manipulation functions. */
#include  "vperror.h"			/* VPERROR() definitions. */
#include  "rex_util.h"			/* Regular expression definitions. */
#include  "rex_internals.h"		/* Internal definitions. */


/*******************************************************************************
    Global variables.
*******************************************************************************/

int  rex_util_debug = 0 ;		/* Debug switch (0 = off, !0 = on). */
char  *rex_error_text = "" ;		/* Message to pinpoint error during parse. */
CompiledRE  rtx = NULL ;		/* Regular expression being parsed. */


/*******************************************************************************
    Private Functions
*******************************************************************************/

static  int  rex_search (
#    if __STDC__
        CompiledRE  pattern,
        int  state,
        const  char  *target,
        int  first,
        int  *next_char,
        int  argFirst[],
        int  argLength[]
#    endif
    ) ;


#ifdef __VMS
/*
 * 29-Aug-1999
 * K.Esteb (WSIPC)
 * kesteb@wsipc.wednet.edu
 *
 * On VMS the DEC copyright symbol is ascii char 215. This caused problems 
 * when trying to match patterns in strings that contained that symbol. The 
 * normal CS_SET(), CS_CLR() and CS_ISSET() macros are not 8 bit clean. 
 * These functions fix that problem.
 */

int CS_SET(int n, struct cs_set *p) {

    if (n < 0) n = (((n * -1) + 128) % 255);
    return(p->css_bits[n/NCSBITS] |= (1 << (n % NCSBITS)));

}

int CS_CLR(int n, struct cs_set *p) {

    if (n < 0) n = (((n * -1) + 128) % 255);
    return(p->css_bits[n/NCSBITS] &= ~(1 << (n % NCSBITS)));

}

int CS_ISSET(int n, struct cs_set *p)	{

    if (n < 0) n = (((n * -1) + 128) % 255);
    return(p->css_bits[n/NCSBITS] & (1 << (n % NCSBITS)));

}
#endif

/*******************************************************************************

Procedure:

    rex_compile ()


Purpose:

    Function REX_COMPILE compiles a regular expression pattern into a form
    suitable for matching (by REX_MATCH).

    NOTE:  REX_MATCH will return TRUE if the regular expression is
    matched anywhere in the target string.  If you want REX_MATCH to
    return TRUE only if the entire target string is matched, anchor
    the match at the beginning and end of the target string by putting
    the '^' and '$' markers at the beginning and end of the regular
    expression you pass into REX_COMPILE.


    Invocation:

        status = rex_compile (expression, &pattern) ;

    where

        <expression>
            is the regular expression to be compiled.
        <pattern>
            returns a handle for the compiled pattern.  This handle
            should be passed into subsequent calls to REX_MATCH.
        <status>
            returns the status of compiling the regular expression, zero if
            no errors occurred and ERRNO otherwise.

*******************************************************************************/


int  rex_compile (

#    if __STDC__
        const  char  *expression,
        CompiledRE  *pattern)
#    else
        expression, pattern)

        char  *expression ;
        CompiledRE  *pattern ;
#    endif

{

    rex_error_text = "" ;

/* Create and initialize a context structure for the regular expression. */

    *pattern = (CompiledRE) malloc (sizeof (_CompiledRE)) ;
    if (*pattern == NULL) {
        vperror ("(rex_compile) Error allocating context node for \"%s\".\nmalloc: ",
                 expression) ;
        rex_error_text = "error allocating context node" ;
        return (errno) ;
    }
    (*pattern)->expression = strdup (expression) ;
    if ((*pattern)->expression == NULL) {
        vperror ("(rex_compile) Error duplicating pattern string \"%s\".\nstrdup: ",
                 expression) ;
        free (*pattern) ;
        rex_error_text = "error duplicating pattern string" ;
        return (errno) ;
    }

    (*pattern)->scan = (*pattern)->expression ;
    (*pattern)->max_states = 0 ;
    (*pattern)->num_states = 0 ;
    (*pattern)->state_list = NULL ;

    (*pattern)->num_args = 0 ;
    (*pattern)->longest_path = 0 ;

/* Compile the regular expression. */

    rtx = *pattern ;		/* Pass the compiled pattern to RRPARSE(). */

    if (rrparse ()) {
        rex_delete (*pattern) ;  *pattern = NULL ;
        return (-1) ;
    } else {
        (*pattern)->bol_anchor =
            ((*pattern)->state_list[(*pattern)->start_state].type == anchor) &&
            ((*pattern)->state_list[(*pattern)->start_state].x.match_char == '^') ;
        return (0) ;
    }

}

/*******************************************************************************

Procedure:

    rex_delete ()


Purpose:

    Function REX_DELETE frees up the memory occupied by a compiled regular
    expression.  REX_DELETE should be called after a program has finished
    using the compiled RE.


    Invocation:

        status = rex_delete (pattern) ;

    where

        <pattern>
            is the handle for the compiled pattern returned by REX_COMPILE().
        <status>
            returns the status of deleting the regular expression, zero if
            no errors occurred and ERRNO otherwise.

*******************************************************************************/


int  rex_delete (

#    if __STDC__
        CompiledRE  pattern)
#    else
        pattern)

        CompiledRE  pattern ;
#    endif

{    /* Local variables. */
    int  i ;



    if (pattern == NULL)  return (0) ;
    if (pattern->state_list != NULL) {
        for (i = 0 ;  i < pattern->num_states ;  i++) {
            if ((pattern->state_list[i].type == match) &&
                (pattern->state_list[i].x.match_char == 0) &&
                (pattern->state_list[i].y.match_charset != NULL))
                free (pattern->state_list[i].y.match_charset) ;
        }
        free (pattern->state_list) ;
    }
    free (pattern->expression) ;
    free (pattern) ;

    return (0) ;

}

/*******************************************************************************

Procedure:

    rex_dump ()


Purpose:

    Function REX_DUMP dumps the contents of a compiled regular expression.


    Invocation:

        status = rex_dump (outfile, pattern) ;

    where

        <outfile>
            is the UNIX file descriptor (FILE *) for the output file.
        <pattern>
            is the handle for the compiled pattern returned by REX_COMPILE().
        <status>
            returns zero.

*******************************************************************************/


int  rex_dump (

#    if __STDC__
        FILE  *outfile,
        CompiledRE  pattern)
#    else
        outfile, pattern)

        FILE  *outfile ;
        CompiledRE  pattern ;
#    endif

{    /* Local variables. */
    int  i ;




    if (pattern == NULL)  return (0) ;

/* Dump the context node. */

    fprintf (outfile, "Regular Expression: \"%s\"    Scan: \"%s\"    BOL Anchor: %s\n",
             pattern->expression, pattern->scan,
             (pattern->bol_anchor ? "yes" : "no")) ;
    fprintf (outfile, "     # of Arguments: %d\n", pattern->num_args) ;
    fprintf (outfile, "       Longest Path: %d\n", pattern->longest_path) ;
    rex_dump_class (outfile, "              First: ", &pattern->first_set) ;
    fprintf (outfile, "Maximum # of states: %d\n", pattern->max_states) ;
    fprintf (outfile, " Actual # of states: %d\n", pattern->num_states) ;
    fprintf (outfile, "        Start state: %d\n", pattern->start_state) ;


/* Dump the list of states. */

    if (pattern->state_list != NULL) {

        for (i = 0 ;  i < pattern->num_states ;  i++) {

            switch (pattern->state_list[i].type) {
            case empty:
                fprintf (outfile, "    State %d - Type: %s  Next: %d %d\n",
                         i, "EMPTY  ", pattern->state_list[i].next1,
                         pattern->state_list[i].next2) ;
                break ;
            case anchor:
                fprintf (outfile, "    State %d - Type: %s  Next: %d %d  Anchor: '%c'\n",
                         i, "ANCHOR ", pattern->state_list[i].next1,
                         pattern->state_list[i].next2,
                         pattern->state_list[i].x.match_char) ;
                break ;
            case alternation:
                fprintf (outfile, "    State %d - Type: %s  Next: %d %d\n",
                         i, "ALTER  ", pattern->state_list[i].next1,
                         pattern->state_list[i].next2) ;
                break ;
            case closure:
                fprintf (outfile, "    State %d - Type: %s  Next: %d %d  Min/Max = %d %d\n",
                         i, "CLOSURE", pattern->state_list[i].next1,
                         pattern->state_list[i].next2,
                         pattern->state_list[i].x.min_closure,
                         pattern->state_list[i].y.max_closure) ;
                break ;
            case final:
                fprintf (outfile, "    State %d - Type: %s\n", i, "FINAL  ") ;
                break ;
            case left_paren:
                fprintf (outfile, "    State %d - Type: %s  Next: %d %d  Subexp Index/State = %d %d\n",
                         i, "LPAREN ", pattern->state_list[i].next1,
                         pattern->state_list[i].next2,
                         pattern->state_list[i].x.subexp_index,
                         pattern->state_list[i].y.subexp_state) ;
                break ;
            case match:
                fprintf (outfile, "    State %d - Type: %s  Next: %d %d  Matches '%c'\n",
                         i, "MATCH  ", pattern->state_list[i].next1,
                         pattern->state_list[i].next2,
                         pattern->state_list[i].x.match_char) ;
                if (pattern->state_list[i].x.match_char == 0)
                    rex_dump_class (outfile, "        Matches: ",
                                    pattern->state_list[i].y.match_charset) ;
                break ;
            case right_paren:
                fprintf (outfile, "    State %d - Type: %s  Next: %d %d  Subexp Index/State = %d %d\n",
                         i, "RPAREN ", pattern->state_list[i].next1,
                         pattern->state_list[i].next2,
                         pattern->state_list[i].x.subexp_index,
                         pattern->state_list[i].y.subexp_state) ;
                break ;
            default:
                fprintf (outfile, "    State %d - Type: %s\n", i, "<?>") ;
                break ;
            }

        }
    }

    return (0) ;

}

/*******************************************************************************

Procedure:

    rex_dump_class ()


Purpose:

    Function REX_DUMP_CLASS dumps a character class.


    Invocation:

        status = rex_dump_class (outfile, lead_in, &char_set) ;

    where

        <outfile>
            is the UNIX file descriptor (FILE *) for the output file.
        <lead_in>
            is the lead-in text for an output dump line.  For each group
            of contiguous characters in the set, the lead-in text is
            output, followed by the starting and ending characters in
            the range of contiguous characters.
        <char_set>
            is a pointer to the character set structure that is to be dumped.
        <status>
            returns zero.

*******************************************************************************/


int  rex_dump_class (

#    if __STDC__
        FILE  *outfile,
        const  char  *lead_in,
        cs_set  *char_set)
#    else
        outfile, lead_in, char_set)

        FILE  *outfile ;
        char  *lead_in ;
        cs_set  *char_set ;
#    endif

{    /* Local variables. */
    int  i, in_range ;



    if (char_set == NULL)  return (0) ;

    in_range = 0 ;
    for (i = 1 ;  i < CS_SETSIZE ;  i++) {
        if (CS_ISSET (i, char_set)) {
            if (!in_range) {
                if (lead_in != NULL)  fprintf (outfile, "%s", lead_in) ;
                if ((i < CS_SETSIZE - 1) && CS_ISSET (i+1, char_set)) {
                    if (isprint ((char) i))
                        fprintf (outfile, "'%c' - ", i) ;
                    else
                        fprintf (outfile, "'\\%3.3o' - ", i) ;
                }
                in_range = 1 ;
            }
            if (i < CS_SETSIZE - 1)  continue ;
        }
        if (in_range) {
            if (isprint ((char) (i-1)))
                fprintf (outfile, "'%c'\n", i - 1) ;
            else
                fprintf (outfile, "'\\%3.3o'\n", i - 1) ;
            in_range = 0 ;
        }
    }

    return (0) ;

}

/*******************************************************************************

Procedure:

    rex_match ()


Purpose:

    Function REX_MATCH tries to match a target string against a compiled
    regular expression.

    NOTE:  REX_MATCH returns TRUE if the regular expression is matched
    anywhere in the target string.  If you want REX_MATCH to return TRUE
    only if the entire target string is matched, anchor the match at the
    beginning and end of the target string by putting the '^' and '$'
    markers at the beginning and end of the regular expression you pass
    into REX_COMPILE.


    Invocation:

        matched = rex_match (target, pattern,
                             &matchStart, &matchLength, numSubExps,
                             [, &subexp0, &length0 [...]]) ;

    where

        <target>
            is the string which will be matched against the regular expression.
        <pattern>
            is the handle for the compiled pattern returned by REX_COMPILE().
        <matchStart>
            returns a pointer to the start of the substring in TARGET that was
            matched by the regular expression; a pointer to TARGET is returned
            if the regular expression was not matched.  This argument can be
            NULL if you're not interested in this information.
        <matchLength>
            returns the length of the substring in TARGET that was matched
            by the regular expression; zero is returned if the regular
            expression was not matched (or if the matched substring was of
            length 0).  This argument can be NULL if you're not interested
            in this information.
        <numSubExps>
            specifies the number of matched subexpressions that the calling
            function wants returned.  Matched subexpressions are specified
            in the regular expression by the "(...)$n" construct,  If
            NUM_SUBEXPS is less than the number called for in the regular
            expression, the trailing subexpressions are not returned.  If
            NUM_SUBEXPS is greater than the number called for in the regular
            expression, the trailing arguments in the call to REX_MATCH are
            set to NULL.
        <subexpN>
            if specified, returns a pointer to the start of the N-th (0..9)
            subexpression matched by the regular expression.  SUBEXPn is
            simply a pointer into the target string, TARGET.
        <lengthN>
            if specified, returns the length of the N-th (0..9) subexpression
            matched by the regular expression.
        <matched>
            returns TRUE (a non-zero value) if the target string matches the
            compiled RE; FALSE (zero) is returned if the target was not matched.

*******************************************************************************/


int  rex_match (

#    if __STDC__
        const  char  *target,
        CompiledRE  pattern,
        char  **matchStart,
        int  *matchLength,
        int  numSubExps,
        ...)
#    else
        target, pattern, matchStart, matchLength, numSubExps, va_alist)

        char  *target ;
        CompiledRE  pattern ;
        char  **matchStart ;
        int  *matchLength ;
        int  numSubExps ;
        va_dcl
#    endif

{    /* Local variables. */
    int  first, i, next, status ;
    int  subexpFirst[10], subexpLength[10] ;
    va_list  arg_ptr ;





    if (pattern == NULL)  return (0) ;


/* At each character position in the target string, try matching the regular
   expression.  If the character at the starting character position is not
   in the first set of characters for the regular expression, then skip
   that character position. */

    for (status = 0, first = next = 0 ;  ;  first++) {
        if (!CS_ISSET (target[first], &pattern->first_set)) {
            if (target[first] == '\0')  break ;		/* End-of-string? */
            continue ;					/* No match possible. */
        }
        for (i = 0 ;  i < pattern->num_states ;  i++)	/* Reset closure counts. */
            pattern->state_list[i].z.count = 0 ;
        for (i = 0 ;  i < 10 ;  i++) {			/* Initialization subexpression pointers. */
            subexpFirst[i] = -1 ;  subexpLength[i] = 0 ;
        }
        if (rex_util_debug)  printf ("(rex_match) Scanning \"%s\" for \"%s\"\n",
                                     &target[first], pattern->expression) ;
        status = rex_search (pattern, pattern->start_state, target, first,
                             &next, subexpFirst, subexpLength) ;
        if (status)  break ;			/* Successful match? */
        if (pattern->bol_anchor)  break ;	/* Only match at beginning-of-line? */
        if (target[first] == '\0')  break ;	/* End-of-line? */
    }

    if (!status)  first = next = 0 ;
    if (matchStart != NULL)  *matchStart = (char *) &target[first] ;
    if (matchLength != NULL)  *matchLength = next - first ;


/* Return matched subexpressions. */

#if __STDC__
    va_start (arg_ptr, numSubExps) ;
#else
    va_start (arg_ptr) ;
#endif

    for (i = 0 ;  (i < numSubExps) && (i < 10) ;  i++) {
        matchStart = va_arg (arg_ptr, char **) ;
        if (matchStart != NULL) {
            *matchStart = (subexpFirst[i] < 0)
                          ? NULL : (char *) (target + subexpFirst[i]) ;
        }
        matchLength = va_arg (arg_ptr, int *) ;
        if (matchLength != NULL)  *matchLength = subexpLength[i] ;
    }

    va_end (arg_ptr) ;

    return (status) ;

}

/*******************************************************************************

Procedure:

    rex_nosex ()


Purpose:

    Function REX_NOSEX returns the number of subexpression assignments
    ("(...)$n" constructs) defined for a compiled regular expression.


    Invocation:

        numSubExps = rex_nosex (pattern) ;

    where

        <pattern>
            is the handle for the compiled pattern returned by REX_COMPILE().
        <numSubExps>
            returns the number (0..10) of subexpression assignments defined
            for the regular expression.  0 is returned in case of an error.

*******************************************************************************/


int  rex_nosex (

#    if __STDC__
        CompiledRE  pattern)
#    else
        pattern)

        CompiledRE  pattern ;
#    endif

{
    if (pattern == NULL)
        return (0) ;
    else
        return (pattern->num_args) ;
}

/*******************************************************************************

Procedure:

    rex_replace ()


Purpose:

    Function REX_REPLACE performs a search-and-replace operation on a
    string of text.  The search string (specified by a previously-compiled
    regular expression) is located in the source string and replaced by the
    substitution text.  This process may be repeated a specified number of
    times.  REX_REPLACE is intended to perform EX(1)-style subsitutions

                          s/<RE>/<replacement>/[g]

    on a line of text.  For example:

        s/abc/def/	(Replaces the first occurrence of "abc" with "def".)
        s/abc/xyz/g	(Replaces all occurrences of "abc" with "xyz".)

    In line with the LEX(1)-style origins of the REX_UTIL package,
    REX_REPLACE uses a notation for its replacement text different
    than that used by other regular expression packages.  The following
    character sequences in replacement text cause the specified actions:

        $0 - $9    Insert subexpression (0..9) matched by RE.
        $&         Insert entire text matched by RE.
        $l0 - $l9  Insert subexpression (0..9) matched by RE,
                   converted to lower case.
        $l&        Insert entire text matched by RE, converted
                   to lower case.
        $u0 - $u9  Insert subexpression (0..9) matched by RE,
                   converted to upper case.
        $u&        Insert entire text matched by RE, converted
                   to upper case.
        \c         Insert character 'c' (e.g., "\$" gives "$").

    For example, applying the following substitution command:

        s/(abc)$1(def)$2/$2$1 is $& split in half and reversed/

    to source string "abcdef" produces this result:

        "defabc is abcdef split in half and reversed"

    REX_REPLACE is probably not very fast - I'm just pleased it works!


    Invocation:

        status = rex_replace (source, pattern, replacement, maxSubstitutions,
                              &result, &numSubstitutions) ;

    where

        <source>
            is the text string to which the search-and-replace operation
            is to be applied.
        <pattern>
            is the handle for the compiled pattern returned by REX_COMPILE().
        <replacement>
            is a string specifying the substitutions to be made in the
            source string when the search string is matched.  See the
            description of special character sequences above.
        <maxSubstitutions>
            specifies the maximum number (0..N) of substitutions that are
            to be made in the source string.  A value of -1 causes global
            substitutions in the source string.  Substitutions are not
            recursive; the search for the next occurrence of the search
            string in the source string begins following the end of the
            last match.
        <result>
            returns the result of substituting the replacement text in the
            source string.  RESULT returns a pointer to a dynamically-allocated
            string of the necessary length.  The calling program is responsible
            for FREE(2)ing the result string.
        <numSubstitutions>
            returns the actual number of substitutions (0..N) made in the
            source string.  This argument may be NULL if you're not interested
            in the return value.
        <status>
            returns the status of performing the search-and-replace operation,
            zero if no errors occurred (whether or not any substitutions were
            made) and ERRNO otherwise (e.g., unable to MALLOC(3) space for the
            RESULT string).

*******************************************************************************/


int  rex_replace (

#    if __STDC__
        const  char  *source,
        CompiledRE  pattern,
        const  char  *replacement,
        int  maxSubstitutions,		/* -1 = throughout source string. */
        char  **result,
        int  *numSubstitutions)
#    else
        source, pattern, replacement, maxSubstitutions,
        result, numSubstitutions)

        char  *source ;
        CompiledRE  pattern ;
        char  *replacement ;
        int  maxSubstitutions ;		/* -1 = throughout source string. */
        char  **result ;
        int  *numSubstitutions ;
#    endif

{    /* Local variables. */
    char  *fieldStart[10], *matchStart, number[2], *r ;
    const  char  *s ;
    int  changeCase, fieldLength[10], i ;
    int  matchLength, matchOffset, numSubs ;
    int  replacementLength, resultLength ;




    for (i = 0 ;  i < 10 ;  i++) {	/* Initialize *unused* fields to zero. */
        fieldStart[i] = (char *) NULL ;  fieldLength[i] = 0 ;
    }
    if (numSubstitutions == (int *) NULL)  numSubstitutions = &numSubs ;


/* For the requested number of substitutions or until no more substitutions
   can be made, match the regular expression in the current result string
   (initially the source string) and substitute the replacement text for the
   matched text.  Substitutions are not recursive: after each substitution,
   the attempt at matching the regular expression begins following the
   substituted text. */

    if (maxSubstitutions <= 0)  maxSubstitutions = strlen (source) ;
    *result = strdup (source) ;
    if (*result == NULL) {
        vperror ("(rex_replace) Error duplicating source string, \"%s\"\nstrdup: ",
                 source) ;
        return (errno) ;
    }
    matchStart = *result ;  matchLength = 0 ;  matchOffset = 0 ;
    replacementLength = 0 ;

    for (*numSubstitutions = 0 ;  *numSubstitutions < maxSubstitutions ;
         (*numSubstitutions)++) {


/* Try matching the regular expression with the text following the last match. */

        source = *result + matchOffset + replacementLength ;
        if (rex_util_debug)  printf ("(rex_replace) Result = \"%s\"  Source = \"%s\"\n",
                                     *result, source) ;
        if (!rex_match (source, pattern, &matchStart, &matchLength,
                        rex_nosex (pattern),
                        &fieldStart[0], &fieldLength[0],
                        &fieldStart[1], &fieldLength[1],
                        &fieldStart[2], &fieldLength[2],
                        &fieldStart[3], &fieldLength[3],
                        &fieldStart[4], &fieldLength[4],
                        &fieldStart[5], &fieldLength[5],
                        &fieldStart[6], &fieldLength[6],
                        &fieldStart[7], &fieldLength[7],
                        &fieldStart[8], &fieldLength[8],
                        &fieldStart[9], &fieldLength[9])) {
            break ;		/* No match. */
        }
        matchOffset = matchStart - *result ;


/* Determine the length of the replacement text: (1) length of unadulterated
   text, (2) plus lengths of referenced subfields, (3) minus length of matched
   text. */

        replacementLength = 0 ;
        for (s = replacement ;  *s != '\0' ;  s++) {
            switch (*s) {
            case '\\':
                if (*++s == '\0')  s-- ;	/* Treat as literal at end-of-line. */
                replacementLength++ ;
                break ;
            case '$':
                if (*++s == '\0')  s-- ;	/* Treat as literal at end-of-line. */
                switch (*s) {
                case 'l':			/* Convert to lower case? */
                    s++ ;  changeCase = -1 ;  break;
                case 'u':			/* Convert to upper case? */
                    s++ ;  changeCase = 1 ;  break;
                default:
                    changeCase = 0 ;  break ;
                }
                if (isdigit (*s)) {		/* Add in length of subfield. */
                    number[0] = *s ;  number[1] = '\0' ;
                    replacementLength = replacementLength +
                                        fieldLength[atoi (number)] ;
                } else if (*s == '&') {		/* Add in length of matched text. */
                    replacementLength = replacementLength + matchLength ;
                } else {
                    replacementLength = replacementLength +
                                        (changeCase ? 3 : 2) ;
                    if (*s == '\0') {		/* "$", "$u", "$l" at end-of-line? */
                        replacementLength-- ;  s-- ;
                    }
                }
                break ;
            default:
                replacementLength++ ;		/* Arbitrary character. */
                break ;
            }
        }     /* Determine length of replacement text. */


/* Allocate a result string of sufficient size to hold the unmatched text and
   the replacement text.  Then, copy the unmatched parts of the source string
   into the result string, leaving a hole for the replacement text. */

        resultLength = strlen (*result) - matchLength + replacementLength ;
        r = strndup (NULL, resultLength) ;
        if (r == NULL) {
            vperror ("(rex_replace) Error allocating %d bytes for result string.\nstrndup: ",
                     resultLength) ;
            return (errno) ;
        }
        str_copy (*result, matchOffset, r, resultLength+1) ;	/* Pre-match text. */
        str_copy (*result + matchOffset + matchLength,		/* Post-match text. */
                 -1,
                 r + matchOffset + replacementLength,
                 resultLength+1 - matchOffset - replacementLength) ;
        source = *result ;
        *result = r ;


/* Finally, substitute the replacement text for the matched text in the result
   string. */

        r = *result + matchOffset ;
        for (s = replacement ;  *s != '\0' ;  s++) {
            switch (*s) {
            case '\\':
                if (*++s == '\0')  s-- ;	/* Treat as literal at end-of-line. */
                *r++ = *s ;
                break ;
            case '$':
                if (*++s == '\0')  s-- ;	/* Treat as literal at end-of-line. */
                switch (*s) {
                case 'l':			/* Convert to lower case? */
                    s++ ;  changeCase = -1 ;  break;
                case 'u':			/* Convert to upper case? */
                    s++ ;  changeCase = 1 ;  break;
                default:
                    changeCase = 0 ;  break ;
                }
                if (isdigit (*s)) {		/* Insert subfield. */
                    number[0] = *s ;  number[1] = '\0' ;
                    i = atoi (number) ;
                    if ((fieldLength[i] > 0) && (fieldStart[i] != NULL)) {
                        strncpy (r, fieldStart[i], fieldLength[i]) ;
                        if (changeCase < 0)
                            str_tolower (r, fieldLength[i]) ;
                        else if (changeCase > 0)
                            str_toupper (r, fieldLength[i]) ;
                        r = r + fieldLength[i] ;
                    }
                } else if (*s == '&') {		/* Insert entire matched text. */
                    if (matchLength > 0) {
                        strncpy (r, matchStart, matchLength) ;
                        if (changeCase < 0)
                            str_tolower (r, matchLength) ;
                        else if (changeCase > 0)
                            str_toupper (r, matchLength) ;
                        r = r + matchLength ;
                    }
                } else {
                    *r++ = '$' ;
                    if (changeCase)
                        s = s - 2 ;		/* Back up to pick up "l" or "u". */
                    else if (*s == '\0')
                        s-- ;			/* Back up to pick up string terminator. */
                }
                break ;
            default:
                *r++ = *s ;			/* Arbitrary character. */
                break ;
            }
        }     /* Substitute replacement text in result string. */


        free ((char *) source) ;


    }     /* For each substitution. */


    return (0) ;

}

/*******************************************************************************

Procedure:

    rex_wild ()


Purpose:

    Function REX_WILD converts a UNIX CSH(1)-style wildcard expression to
    the corresponding regular expression (RE).  The RE conversion handles
    the following cases:

        "^", RE's start-of-string symbol, is prepended to the RE to anchor the
            pattern match at the beginning of the target string.

        "*" in the wildcard expression is converted to ".*" in the RE.  "." is
            a special RE symbol that matches any character and "*" indicates
            zero or more occurrences of the preceding character, so ".*" matches
            zero or more characters in the target string.

        "?" in the wildcard expression is converted to "." in the RE.  "." is
            a special RE symbol that matches any single character.

        "." in the wildcard expression converted to "\." (escaped dot) in the
            RE to disambiguate it from RE's special symbol.

        All other characters are passed through as is (meaning they must occur
            in the target file name).  NOTE that a shell character list/range
            enclosed in square brackets ("[...]") is passed through as is; the
            shell character set specification is compatible with the regular
            expression character set specification.

        "$", RE's end-of-string symbol, is appended to the RE so that execution
            of the RE must consume the entire target string.

    For example, the following wildcard expression:

                             "*.c.0*"

    (which matches strings like "fsearch.c.007") will be converted into:

                           ".*\.c\.0.*"


    Invocation:

        regularExpression = rex_wild (wildcardExpression) ;

    where

        <wildcardExpression>
            is the wildcard expression to be converted to a regular expression.
        <regularExpression>
            returns a pointer to a string containing the converted regular
            expression.  This string, which can be passed to REX_COMPILE(),
            is stored in memory local to REX_WILD() and must be used or
            duplicated before calling REX_WILD() again.

*******************************************************************************/


char  *rex_wild (

#    if __STDC__
        const  char  *wildcard)
#    else
        wildcard)

        char  *wildcard ;
#    endif

{    /* Local variables. */
    char  *r ;
    const  char  *s ;
    static  char  regularExpression[1024] ;



    memset (regularExpression, '\0', (sizeof regularExpression)) ;
    r = regularExpression ;

    *r++ = '^' ;			/* Anchor match at start-of-line. */

    for (s = wildcard ;  *s != '\0' ;  r++, s++) {
        if (*s == '*')
            strcpy (r++, ".*") ;	/* Match 0 or more of any character. */
        else if (*s == '?')
            strcpy (r, ".") ;		/* Match any single character. */
        else if (*s == '.')
            strcpy (r++, "\\.") ;	/* Match a period. */
        else
            *r = *s ;			/* Match this character exactly. */
    }

    *r++ = '$' ;			/* Match must extend to end-of-line. */

    return (regularExpression) ;

}

/*******************************************************************************

Procedure:

    rex_search ()


Purpose:

    Function REX_SEARCH tries to match a target string against a compiled
    regular expression by searching the graph that represents the regular
    expression.

    Two implementations of REX_SEARCH() are provided below.  The recursive
    form of REX_SEARCH is used if REX_UTIL.C is compiled with the
    "-DRECURSIVE" C Preprocessor flag; the iterative form of REX_SEARCH()
    is compiled if the "-DRECURSIVE" flag is not specified.  Not
    unexpectedly, the recursive version of REX_SEARCH() is slower than
    the iterative version.  The recursive form is more easily understood,
    however, so it was retained for testing purposes.

    In the iterative form of REX_SEARCH(), a stack is used to simulate the
    recursion.  Information is only pushed on the stack when REX_SEARCH()
    is following the NEXT1 link of an alternation node or the NEXT2 link
    of a closure node.  The other "recursive" calls to REX_SEARCH() are
    examples of tail recursion, where the current instantiation of
    REX_SEARCH() returns immediately after calling itself recursively.
    In these cases, a loop is as good as a push-pop.


    NOTE:  Before each call to REX_SEARCH() by REX_MATCH(), the COUNT
           fields in the state nodes must be initialized to zero.


    Invocation:

        matched = rex_search (pattern, state, target, first_char,
                              &next_char, &argFirst[], &argLength[]) ;

    where

        <pattern>
            is the handle for the compiled pattern returned by REX_COMPILE().
        <state>
            is the start state of the regular expression that will be used
            to match the target string.  This argument is only used by the
            recursive version of REX_SEARCH().
        <target>
            is the string which will be matched against the regular expression.
        <first_char>
            is the offset of the character in TARGET with which REX_SEARCH()
            begins its search for a match.
        <next_char>
            returns, if the match was successful, the offset of the next
            character following the last character matched by the regular
            expression.
        <argFirst>
            is the address of an array of integers that receives the offsets
            of the first character in matched subexpressions (specified using
            the "(...)$n" construct in the regular expresssion).
        <argLength>
            is the address of an array of integers that receives the lengths
            of matched subexpressions (specified using the "(...)$n" construct
            in the regular expresssion).
        <matched>
            returns TRUE (a non-zero value) if the target string matches the
            compiled RE; FALSE (zero) is returned if the target was not matched.

*******************************************************************************/



static  int  rex_search (

#    if __STDC__
        CompiledRE  pattern,
        int  state,
        const  char  *target,
        int  first,
        int  *next_char,
        int  argFirst[],
        int  argLength[])
#    else
        pattern, state, target, first, next_char, argFirst, argLength)

        CompiledRE  pattern ;
        int  state ;
        char  *target ;
        int  first ;
        int  *next_char ;
        int  argFirst[] ;
        int  argLength[] ;
#    endif

{    /* Local variables. */
    int  count = 0, i ;
    StateNode  *slist ;

#ifndef  RECURSIVE
    int  branch, popped_from_stack, sp ;

    typedef  struct  StackEntry {
        int  state, offset, count, branch ;
    } StackEntry ;
    static  StackEntry  *stack = NULL ;
    static  int  maxStackSize = 0 ;
#endif


#ifdef  RECURSIVE


/* Traverse the graph representation of the regular expressions, attempting to
   match matching character nodes to characters in the target string. */

    *next_char = first ;
    slist = pattern->state_list ;
    if (rex_util_debug)
        printf ("(rex_search) State = %d  First = %d  Count = %d\n",
                state, first, (state < 0) ? 0 : slist[state].z.count) ;
    if (state < 0)  return (0) ;


    switch (slist[state].type) {

    case empty:				/* Extend search on NEXT1 branch. */
        return (rex_search (pattern, slist[state].next1, target, first,
                            next_char, argFirst, argLength)) ;

    case anchor:			/* If anchored correctly, extend search on NEXT1 branch. */
        if (((slist[state].x.match_char == '^') && (first == 0)) ||
            ((slist[state].x.match_char == '$') && (target[first] == '\0')))
            return (rex_search (pattern, slist[state].next1, target, first,
                                next_char, argFirst, argLength)) ;
        else
            return (0) ;

    case alternation:			/* Try one branch, then the other. */
        return (rex_search (pattern, slist[state].next1, target, first,
                            next_char, argFirst, argLength)  ||
                rex_search (pattern, slist[state].next2, target, first,
                            next_char, argFirst, argLength)) ;
    case closure:
        count = slist[state].z.count ;	/* Save current count from state. */
        if (slist[state].z.count < slist[state].y.max_closure) {
            slist[state].z.count++ ;	/* Attempt closure on NEXT2 branch. */
            if (rex_search (pattern, slist[state].next2, target, first,
                            next_char, argFirst, argLength))  return (1) ;
            slist[state].z.count = count ;	/* Restore state's count. */
            if (count < slist[state].x.min_closure)
                return (0) ;		/* Insufficient # of repetitions - terminate this branch of search. */
        } else {
            slist[state].z.count = 0 ;	/* Maximum # of repetitions - extend search on NEXT1 branch. */
        }
		/* NEXT2 branch failed or maximum # of repetitions reached - extend search on NEXT1 branch. */
        slist[state].z.count = 0 ;
        if (rex_search (pattern, slist[state].next1, target, first,
                        next_char, argFirst, argLength))  return (1) ;
        slist[state].z.count = count ;	/* Restore state's count and return failure. */
       return (0) ;

    case final:
        *next_char = first ;		/* Success! */
        return (1) ;

    case match:
        if (target[first] == '\0') {
            return (0) ;		/* End of target string - return. */
        } else if ((slist[state].x.match_char < 0) ||
                   (target[first] == slist[state].x.match_char) ||
                   ((slist[state].x.match_char == 0) &&
                    isascii (target[first]) &&
                    (CS_ISSET (target[first], slist[state].y.match_charset)))) {
            first++ ;			/* Character matches - follow NEXT1 branch. */
            return (rex_search (pattern, slist[state].next1, target, first,
                                next_char, argFirst, argLength)) ;
        } else {
            return (0) ;		/* Character doesn't match - return. */
        }

    case left_paren:
        i = slist[state].x.subexp_index ;
        argFirst[i] = first ;		/* Save start of text and extend search on NEXT1 branch. */
        if (rex_search (pattern, slist[state].next1, target, first,
                        next_char, argFirst, argLength)) {
            return (1) ;
        } else {
            argFirst[i] = -1 ;		/* Search failed - erase argument. */
            return (0) ;
        }

    case right_paren:
        i = slist[state].x.subexp_index ;
        argLength[i] = first - argFirst[i] ;	/* Save end of text and extend search on NEXT1 branch. */
        return (rex_search (pattern, slist[state].next1, target, first,
                            next_char, argFirst, argLength)) ;

    default:
        break ;

    }

    return (0) ;		/* Not found. */


#else

#define  push(s, o, c, b) \
    ((sp < maxStackSize) ? \
     (stack[sp].state = s, stack[sp].offset = o, stack[sp].count = c, stack[sp++].branch = b, 0) : 1)
#define  pop(s, o, c, b) \
    ((sp > 0) ? \
     (s = stack[--sp].state, o = stack[sp].offset, c = stack[sp].count, b = stack[sp].branch, 0) : 1)


/* Allocate a stack for simulating the recursive graph search.  The maximum
   number of entries allocated for the stack is equal to the length of the
   target string plus the length of the longest path in the RE graph.  Match
   nodes and parentheses nodes do not consume stack space; only alternation
   and closure nodes do.  Consequently, the size of the run-time stack (i.e.,
   the number of alternation/closure nodes traversed) will be proportional
   to the number of characters in the target string and the sizing of the
   stack should be sufficient.  Keep your fingers crossed! */

    i = pattern->longest_path + strlen (target) ;
    if ((i > maxStackSize) || (stack == NULL)) {
        if (stack != NULL)  free (stack) ;
        stack = (StackEntry *) calloc (i, sizeof (StackEntry)) ;
        if (stack == NULL) {
            vperror ("(rex_search) Error allocating stack storage for \"%s\".\ncalloc: ", target) ;
            return (0) ;
        }
        maxStackSize = i ;
    }


/* Traverse the graph representation of the regular expressions, attempting to
   match matching character nodes to characters in the target string. */

    *next_char = first ;
    state = pattern->start_state ;
    branch = 0 ;  popped_from_stack = 0 ;  sp = 0 ;
    slist = pattern->state_list ;

    while (state >= 0) {

        if (rex_util_debug)
            printf ("(rex_search)%*sState = %d  First = %d  Count = %d  Branch = %d  SP = %d%s\n",
                    (sp+1)*2, " ", state, first,
                    (popped_from_stack ? count : slist[state].z.count),
                    branch, sp, (popped_from_stack ? " *" : "")) ;

        switch (slist[state].type) {

        case empty:
            state = slist[state].next1 ;		/* Extend search on NEXT1 branch. */
            break ;

        case anchor:
            if (((slist[state].x.match_char == '^') && (first == 0)) ||
                ((slist[state].x.match_char == '$') && (target[first] == '\0')))
                state = slist[state].next1 ;		/* Extend search on NEXT1 path. */
            else
                state = -1 ;
            break ;

        case alternation:
            if (popped_from_stack) {
                state = slist[state].next2 ;		/* Extend search on NEXT2 path. */
            } else {
                if (push (state, first, 0, 1)) {
                    fprintf (stderr, "(rex_search) Stack overflow on RE \"%s\", target \"%s\"\n",
                             pattern->expression, target) ;
                    return (0) ;
                }
                state = slist[state].next1 ;		/* Extend search on NEXT1 path. */
            }
            break ;

        case closure:
            if (popped_from_stack) {
                slist[state].z.count = count ;		/* Restore state's count. */
                if (count < slist[state].x.min_closure) {
                    state = -1 ;			/* Insufficient # of repetitions - terminate this branch of search. */
                } else if (count < slist[state].y.max_closure) {
                    if (branch == 2) {
                        if (push (state, first, count, 1)) {
                            fprintf (stderr, "(rex_search) Stack overflow on RE \"%s\", target \"%s\"\n",
                                     pattern->expression, target) ;
                            return (0) ;
                        }
                        slist[state].z.count = 0 ;
                        state = slist[state].next1 ;	/* NEXT2 branch failed - extend search on NEXT1 path. */
                    } else {
                        state = -1 ;			/* NEXT1 branch failed, too. */
                    }
                } else {
                    state = -1 ;			/* Maximum # of repetitions - search on NEXT1 path failed. */
                }
            } else {
                count = slist[state].z.count ;		/* Save current count from state. */
                if (slist[state].z.count < slist[state].y.max_closure) {
                    slist[state].z.count++ ;  branch = 2 ;
                    i = slist[state].next2 ;		/* Attempt closure on NEXT2 path. */
                } else {
                    slist[state].z.count = 0 ;  branch = 1 ;
                    i = slist[state].next1 ;		/* Maximum # of repetitions - extend search on NEXT1 path. */
                }
                if (push (state, first, count, branch)) {
                    fprintf (stderr, "(rex_search) Stack overflow on RE \"%s\", target \"%s\"\n",
                             pattern->expression, target) ;
                    return (0) ;
                }
                state = i ;
            }
            break ;

        case final:
            *next_char = first ;			/* Success! */
            return (1) ;

        case match:
            if (target[first] == '\0') {
                state = -1 ;				/* End of target string - return. */
            } else if ((slist[state].x.match_char < 0) ||
                       (target[first] == slist[state].x.match_char) ||
                       ((slist[state].x.match_char == 0) &&
                        isascii (target[first]) &&
                        (CS_ISSET (target[first], slist[state].y.match_charset)))) {
                first++ ;
                state = slist[state].next1 ;		/* Character matches - follow NEXT1 branch. */
            } else {
                state = -1 ;				/* Character doesn't match - return. */
            }
            break ;

        case left_paren:
            i = slist[state].x.subexp_index ;
            if (popped_from_stack) {
                argFirst[i] = -1 ;			/* Search failed - erase argument. */
                state = -1 ;
            } else {
                if (push (state, first, 0, 1)) {
                    fprintf (stderr, "(rex_search) Stack overflow on RE \"%s\", target \"%s\"\n",
                             pattern->expression, target) ;
                    return (0) ;
                }
                argFirst[i] = first ;			/* Save start of text. */
                state = slist[state].next1 ;		/* Extend search on NEXT1 branch. */
            }
            break ;

        case right_paren:
            i = slist[state].x.subexp_index ;		/* Save end of text. */
            argLength[i] = first - argFirst[i] ;
            state = slist[state].next1 ;		/* Extend search on NEXT1 branch. */
            break ;

        default:
            break ;

        }

        popped_from_stack = 0 ;
        while (state < 0) {			/* "Return" from recursive calls. */
            if (pop (state, first, count, branch))  break ;
            popped_from_stack = 1 ;
        }

    }

    return (0) ;		/* Not found. */

#endif  /* RECURSIVE */

}
