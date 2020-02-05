#ifndef lint
static char rrsccsid[] = "@(#)yaccpar	1.9 (Berkeley) 02/21/93";
#endif
#define RRBYACC 1
#define RRMAJOR 1
#define RRMINOR 9
#define rrclearin (rrchar=(-1))
#define rrerrok (rrerrflag=0)
#define RRRECOVERING (rrerrflag!=0)
#define RRPREFIX "rr"
#line 2 "rex_util_y.y"
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
@(#)  FILE: rex_util_y.y  RELEASE: 1.8  DATE: 07/12/96, 15:35:53
*/
/*******************************************************************************

Procedure:

    rrparse ()


Author:    Alex Measday, ISI


Purpose:

    Function RRPARSE is a YACC-generated routine that parses a regular
    expression (RE) and builds a non-deterministic finite state machine
    that will recognize the RE.  The grammar for regular expressions
    is derived from the grammar presented by Robert Sedgewick in his
    book, ALGORITHMS (Chapter 21: Parsing).


    Invocation:

        status = rrparse () ;

    where

        <status>
            returns the status of parsing the input, zero if no errors occurred
            and a non-zero value otherwise.


    Public Variables:

        REX_UTIL_ERROR_TEXT - is a (CHAR *) pointer to a short error message
            explaining why RRPARSE failed, if it does.


Development History:
                                               Description
Author         Change ID       Build/Date      of Change
-------------  --------------  --------------  ---------------------
A. Measday     Port from TPOCC Release 6.1 to XSAR Release 0
    Added conditional inclusion of VMS-specific header files.  Eliminated
    external function declarations, since the required function prototypes
    are available in the "#include"d system and TPOCC header files.

A. Measday     Port from TPOCC Release 7 to XSAR Pre-Release 1
    Converted function declarations to ANSI C and added function prototypes
    for static, internal functions.

*******************************************************************************/

#define  _SVID_SOURCE  1

#include  <ctype.h>			/* Character classification macros. */
#include  <errno.h>			/* System error definitions. */
#include  <limits.h>			/* Maximum/minimum value definitions. */
#include  <stdio.h>			/* Standard I/O definitions. */
#include  <stdlib.h>			/* Standard C Library definitions. */
#include  <string.h>			/* Standard C string functions. */
#include  "str_util.h"			/* String manipulation functions. */
#include  "vperror.h"			/* VPERROR() definitions. */
#include  "rex_util.h"			/* Regular expression definitions. */
#include  "rex_internals.h"		/* Internal definitions. */

#define  NEW_STATE \
  { if (allocate_state ()) { \
        rex_error_text = "error increasing size of state list" ; \
        return (errno) ; \
    } \
  }

#define  display_production(left, right) \
    if (rex_util_debug)  printf ("%s ::== %s\n", left, right) ;

#define  display_token(name, text) \
    { if (rex_util_debug)  printf ("-- Token %s = \"%c\"\n", name, text) ; }

#define  beginning_of_input()  (rtx->scan == rtx->expression)
#define  input()  ((*rtx->scan == '\0') ? 0 : *rtx->scan++)
#define  unput(c)  (((rtx->scan == rtx->expression) || (c == '\0')) ? -1 : (rtx->scan--, 0))

/*******************************************************************************

    Internal Non-Local Variables.  These flags give the parser (RRPARSE) some
        control over how the lexer (RRLEX) interprets characters in the input
        string.  The flags are automatically reset when RRLEX is positioned at
        the beginning of the input string.

        RRPARSE_AFTER_BRACKET - controls the interpretation of characters
            within a character class specification (i.e., "[...]").  A value
            of zero indicates that the parser is not in the midst of a bracket
            expression.  Once the left bracket is encountered, RRPARSE sets
            this variable to 1; as each character in the bracket is scanned,
            RRPARSE_AFTER_BRACKET is incremented.

        RRPARSE_EXPECTING_ASSIGNMENT - controls the interpretation of a '$'
            that follows a right parenthesis: "...)$...".  RRPARSE sets this
            variable to 1 after reading the right parenthesis.  If RRLEX then
            encounters a '$' followed by a digit ('0' - '9'), it returns the
            '$' as an ASSIGNMENT token; otherwise, the '$' is returned as an
            EOL_ANCHOR token.

        RRPARSE_HYPHEN_AS_LITERAL - controls the interpretation of hyphens
            within a bracketed character class expression.

*******************************************************************************/

static  int  rrparse_after_bracket = 0 ;
static  int  rrparse_expecting_assignment = 0 ;
static  int  rrparse_hyphen_as_literal = 0 ;


/*******************************************************************************
    Private Functions
*******************************************************************************/

static  int  rrerror (
#    if __STDC__ || defined(vaxc)
        char  *s
#    endif
    ) ;

static  int  rrlex (
#    if __STDC__ || defined(vaxc)
        void
#    endif
    ) ;

static  int  allocate_state (
#    if __STDC__ || defined(vaxc)
        void
#    endif
    ) ;

static  int  first_char_of (
#    if __STDC__ || defined(vaxc)
        int  state,
        cs_set  *first_set
#    endif
    ) ;

static  int  last_of (
#    if __STDC__ || defined(vaxc)
        int  state
#    endif
    ) ;

static  int  longest_path (
#    if __STDC__ || defined(vaxc)
        int  state
#    endif
    ) ;

static  int  shortest_path (
#    if __STDC__ || defined(vaxc)
        int  state
#    endif
    ) ;
#line 211 "rex_util_y.y"
/*******************************************************************************
    Miscellaneous declarations.
*******************************************************************************/

    static  char  buffer[8] ;
    static  cs_set  char_class ;
    static  int  i ;

/* Character classification names.  The ordering of these names is important
   to RRPARSE; see the "character_class => CTYPE" rule before making any
   changes. */

    static  char  *class_name[] = {
        "alnum",
        "alpha",
        "cntrl",
        "digit",
        "graph",
        "lower",
        "print",
        "punct",
        "space",
        "upper",
        "xdigit",
        NULL
    } ;

#line 213 "y.tab.c"
#define ANY_CHARACTER 257
#define ASSIGNMENT 258
#define BOL_ANCHOR 259
#define CARAT 260
#define COMMA 261
#define CTYPE 262
#define DIGIT 263
#define EOL_ANCHOR 264
#define _ERROR 265
#define HYPHEN 266
#define LEFT_BRACE 267
#define LEFT_BRACKET 268
#define LEFT_PAREN 269
#define ONE_OR_MORE 270
#define OR 271
#define RIGHT_BRACE 272
#define RIGHT_BRACKET 273
#define RIGHT_PAREN 274
#define SINGLE_CHARACTER 275
#define ZERO_OR_MORE 276
#define ZERO_OR_ONE 277
#define RRERRCODE 256
short rrlhs[] = {                                        -1,
    0,    1,    1,    1,    2,    2,    3,    3,    3,    7,
    3,    3,    3,    3,    9,    3,    3,    3,    3,    3,
    6,    6,    8,    8,    4,    4,    5,    5,   11,   11,
   10,   10,   12,   14,   12,   12,   13,
};
short rrlen[] = {                                         2,
    1,    0,    1,    3,    1,    2,    1,    1,    5,    0,
    5,    2,    2,    2,    0,    5,    1,    1,    1,    1,
    0,    2,    0,    1,    0,    1,    0,    2,    1,    2,
    1,    2,    1,    0,    4,    1,    1,
};
short rrdefred[] = {                                      0,
   20,    7,   18,   19,    8,   15,    0,   17,    0,    0,
    3,    0,    0,    0,    0,    0,   13,   12,   14,    6,
   24,    0,   10,    4,   29,    0,    0,   36,   37,    0,
   31,    0,    0,    0,    0,   30,   16,   32,   34,    0,
   11,   28,    9,    0,   22,   35,
};
short rrdgoto[] = {                                       9,
   10,   11,   12,   26,   35,   41,   33,   22,   13,   30,
   27,   31,   32,   44,
};
short rrsindex[] = {                                   -226,
    0,    0,    0,    0,    0,    0, -226,    0,    0, -268,
    0, -247, -233, -267, -226, -224,    0,    0,    0,    0,
    0, -257,    0,    0,    0, -221, -217,    0,    0, -262,
    0, -218, -208, -224, -220,    0,    0,    0,    0, -212,
    0,    0,    0, -222,    0,    0,
};
short rrrindex[] = {                                      2,
    0,    0,    0,    0,    0,    0, -230,    0,    0,   54,
    0,    8, -256,    0,    9, -246,    0,    0,    0,    0,
    0,    0,    0,    0,    0, -216, -236,    0,    0,    0,
    0, -228,    1, -215,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,
};
short rrgindex[] = {                                      0,
   17,   43,    0,   24,    0,    0,    0,    0,    0,    0,
    0,   29,   16,    0,
};
#define RRTABLESIZE 283
short rrtable[] = {                                      28,
   21,    2,   15,   15,   28,   23,   23,    5,    2,    1,
   37,    2,   29,    3,   25,    4,    5,   29,   23,   16,
    6,    7,   17,   14,   26,   25,   21,    8,   18,   19,
    1,   24,    2,   33,    3,   26,    4,    5,   25,   34,
    2,    6,    7,    2,   33,   36,   33,   39,    8,   40,
   45,   43,   29,    1,   20,   27,   25,   42,   38,   46,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,   21,    0,   21,
    0,   21,    0,   21,   21,    0,    0,   21,   21,   21,
   21,   21,    2,    0,   21,   21,   21,   21,    5,    2,
    0,    5,    2,
};
short rrcheck[] = {                                     262,
    0,    0,  271,  271,  262,  262,  274,    0,    0,  257,
  273,  259,  275,  261,  261,  263,  264,  275,  275,  267,
  268,  269,  270,    7,  261,  272,  260,  275,  276,  277,
  257,   15,  259,  262,  261,  272,  263,  264,  263,  261,
  271,  268,  269,  274,  273,  263,  275,  266,  275,  258,
  263,  272,  275,    0,   12,  272,  272,   34,   30,   44,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  257,   -1,  259,
   -1,  261,   -1,  263,  264,   -1,   -1,  267,  268,  269,
  270,  271,  271,   -1,  274,  275,  276,  277,  271,  271,
   -1,  274,  274,
};
#define RRFINAL 9
#ifndef RRDEBUG
#define RRDEBUG 0
#endif
#define RRMAXTOKEN 277
#if RRDEBUG
char *rrname[] = {
"end-of-file",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,"ANY_CHARACTER","ASSIGNMENT",
"BOL_ANCHOR","CARAT","COMMA","CTYPE","DIGIT","EOL_ANCHOR","_ERROR","HYPHEN",
"LEFT_BRACE","LEFT_BRACKET","LEFT_PAREN","ONE_OR_MORE","OR","RIGHT_BRACE",
"RIGHT_BRACKET","RIGHT_PAREN","SINGLE_CHARACTER","ZERO_OR_MORE","ZERO_OR_ONE",
};
char *rrrule[] = {
"$accept : complete_re",
"complete_re : regular_expression",
"regular_expression :",
"regular_expression : term",
"regular_expression : regular_expression OR regular_expression",
"term : factor",
"term : factor term",
"factor : BOL_ANCHOR",
"factor : EOL_ANCHOR",
"factor : factor LEFT_BRACE number upper_bound RIGHT_BRACE",
"$$1 :",
"factor : LEFT_PAREN regular_expression RIGHT_PAREN $$1 assignment",
"factor : factor ZERO_OR_MORE",
"factor : factor ONE_OR_MORE",
"factor : factor ZERO_OR_ONE",
"$$2 :",
"factor : LEFT_BRACKET $$2 complement character_classes RIGHT_BRACKET",
"factor : SINGLE_CHARACTER",
"factor : COMMA",
"factor : DIGIT",
"factor : ANY_CHARACTER",
"assignment :",
"assignment : ASSIGNMENT DIGIT",
"complement :",
"complement : CARAT",
"number :",
"number : digits",
"upper_bound :",
"upper_bound : COMMA number",
"digits : DIGIT",
"digits : digits DIGIT",
"character_classes : character_class",
"character_classes : character_classes character_class",
"character_class : character",
"$$3 :",
"character_class : character HYPHEN $$3 character",
"character_class : CTYPE",
"character : SINGLE_CHARACTER",
};
#endif
#ifndef RRSTYPE
typedef int RRSTYPE;
#endif
#ifdef RRSTACKSIZE
#undef RRMAXDEPTH
#define RRMAXDEPTH RRSTACKSIZE
#else
#ifdef RRMAXDEPTH
#define RRSTACKSIZE RRMAXDEPTH
#else
#define RRSTACKSIZE 500
#define RRMAXDEPTH 500
#endif
#endif
int rrdebug;
int rrnerrs;
int rrerrflag;
int rrchar;
short *rrssp;
RRSTYPE *rrvsp;
RRSTYPE rrval;
RRSTYPE rrlval;
short rrss[RRSTACKSIZE];
RRSTYPE rrvs[RRSTACKSIZE];
#define rrstacksize RRSTACKSIZE
#line 676 "rex_util_y.y"

/*******************************************************************************
    Function RRERROR is invoked automatically by RRPARSE when an error is
    detected.  RRERROR simply prints out the error message passed to it by
    RRPARSE.
*******************************************************************************/

static  int  rrerror (

#    if __STDC__ || defined(vaxc)
        char  *s)
#    else
        s)

        char  *s ;
#    endif

{
    if (rex_util_debug)  printf ("(rrparse) %s\n", s) ;
    errno = EINVAL ;  rex_error_text = s ;
    return (errno) ;
}

/*******************************************************************************
    Function RRLEX returns the next token of input.  This function is normally
    generated by LEX, but, for this parser, it's simple enough to do by hand.
*******************************************************************************/


static  int  rrlex (

#    if __STDC__ || defined(vaxc)
        void)
#    else
        )
#    endif

{    /* Local variables. */
    char  buffer[8], c ;
    unsigned  int  i ;




    if (beginning_of_input ()) {		/* Reset lexical flags. */
        rrparse_after_bracket = 0 ;
        rrparse_expecting_assignment = 0 ;
        rrparse_hyphen_as_literal = 0 ;
    }

    c = input () ;  rrlval = c ;

/* At the end of the string, return an end marker. */

    if (c == '\0') {
        display_token ("END_MARKER", '0') ;  return (0) ;
    }

/* Within a bracket expression ("[...]"), most characters are interpreted
   literally.  When the left bracket is encountered, RRPARSE sets AFTER_BRACKET
   to 1.  A "^" immediately after the left bracket is returned as the character
   class complement indicator and AFTER_BRACKET is set to -1.  A right bracket
   or hyphen immediately after the left bracket (AFTER_BRACKET == 1) or the
   complement character (AFTER_BRACKET == -1) is returned as a literal
   character.  A hyphen following a hyphen ("[a--]") or preceding the right
   bracket ("[a-]") is returned as a literal character.  A colon (":")
   immediately after the left bracket or the complement character is the
   start of a character class name specification (":<class>:"); the class
   name is extracted, looked up in the class name table, and its index is
   returned to RRPARSE.  "\<c>" constructs are handled by the non-bracket
   expression SWITCH statement further down below. */

    if (rrparse_after_bracket) {
        switch (c) {
        case '^':
            if (rrparse_after_bracket == 1) {
                rrparse_after_bracket = -1 ;
                display_token ("CARAT", c) ;  return (CARAT) ;
            }
            if (rrparse_after_bracket == -1)  rrparse_after_bracket = 1 ;
            rrparse_after_bracket++ ;
            display_token ("SINGLE_CHARACTER", c) ;  return (SINGLE_CHARACTER) ;
        case ']':
            if (rrparse_after_bracket++ > 1) {
                display_token ("RIGHT_BRACKET", c) ;  return (RIGHT_BRACKET) ;
            }
            rrparse_after_bracket = 2 ;
            display_token ("SINGLE_CHARACTER", c) ;  return (SINGLE_CHARACTER) ;
        case '-':
            c = input () ;  unput (c) ;		/* Is "-" followed by a "]"? */
            if ((rrparse_after_bracket++ > 1) &&
                (!rrparse_hyphen_as_literal) && (c != ']')) {
                display_token ("HYPHEN", c) ;  return (HYPHEN) ;
            }
            rrparse_after_bracket = 2 ;
            display_token ("SINGLE_CHARACTER", '-') ;
            return (SINGLE_CHARACTER) ;
        case ':':
            if (rrparse_after_bracket++ > 1) {
                display_token ("SINGLE_CHARACTER", c) ;  return (SINGLE_CHARACTER) ;
            }
            rrparse_after_bracket = 2 ;
				/* Extract class name from ":<class>:". */
            for (i = 0 ;  (c = input ()) && (c != ':') ;  )
                if (i < (sizeof buffer - 1))  buffer[i++] = c ;
            buffer[i] = '\0' ;  str_tolower (buffer, -1) ;
				/* Lookup name in class name table. */
            for (i = 0 ;  class_name[i] != NULL ;  i++)
                if (strcmp (buffer, class_name[i]) == 0)  break ;
            rrlval = i ;  return (CTYPE) ;
        case '\\':
            if (rrparse_after_bracket == -1)  rrparse_after_bracket = 1 ;
            rrparse_after_bracket++ ;  rrparse_after_bracket++ ;
            break ;
        default:
            if (rrparse_after_bracket == -1)  rrparse_after_bracket = 1 ;
            rrparse_after_bracket++ ;
            display_token ("SINGLE_CHARACTER", c) ;  return (SINGLE_CHARACTER) ;
        }
    }


/* Outside of a bracket expression, characters receive the standard regular
   expression interpretation. */

    switch (c) {
    case '.':
        display_token ("ANY_CHARACTER", c) ;  return (ANY_CHARACTER) ;
    case '^':
        display_token ("BOL_ANCHOR", c) ;  return (BOL_ANCHOR) ;
    case ',':
        display_token ("COMMA", c) ;  return (COMMA) ;
    case '$':
        c = input () ;  unput (c) ;
        if (rrparse_expecting_assignment && isdigit (c)) {
            rrparse_expecting_assignment = 0 ;
            display_token ("ASSIGNMENT", '$') ;  return (ASSIGNMENT) ;
        } else {
            rrparse_expecting_assignment = 0 ;
            display_token ("EOL_ANCHOR", '$') ;  return (EOL_ANCHOR) ;
        }
    case '|':
        display_token ("OR", c) ;  return (OR) ;
    case '*':
        display_token ("ZERO_OR_MORE", c) ;  return (ZERO_OR_MORE) ;
    case '+':
        display_token ("ONE_OR_MORE", c) ;  return (ONE_OR_MORE) ;
    case '?':
        display_token ("ZERO_OR_ONE", c) ;  return (ZERO_OR_ONE) ;
    case '(':
        display_token ("LEFT_PAREN", c) ;  return (LEFT_PAREN) ;
    case ')':
        display_token ("RIGHT_PAREN", c) ;  return (RIGHT_PAREN) ;
    case '{':
        display_token ("LEFT_BRACE", c) ;  return (LEFT_BRACE) ;
    case '}':
        display_token ("RIGHT_BRACE", c) ;  return (RIGHT_BRACE) ;
    case '[':
        display_token ("LEFT_BRACKET", c) ;  return (LEFT_BRACKET) ;
    case '\\':
        c = input() ;  rrlval = c ;
        switch (c) {
        case 'n':  rrlval = '\n' ;  break ;
        case 'r':  rrlval = '\n' ;  break ;
        case 't':  rrlval = '\t' ;  break ;
        default:   break ;
        }
        if (c == '\0') {
            unput (c) ;  display_token ("_ERROR", '0') ;  return (_ERROR) ;
        } else {
            display_token ("SINGLE_CHARACTER", c) ;  return (SINGLE_CHARACTER) ;
        }
    default:
        if (isdigit (c)) {
            display_token ("DIGIT", c) ;  return (DIGIT) ;
        } else {
            display_token ("SINGLE_CHARACTER", c) ;  return (SINGLE_CHARACTER) ;
        }
    }

}

/*******************************************************************************
    Function ALLOCATE_STATE simply checks if there is enough room to add a new
    state to the state list.  If there is enough room, nothing is done; the
    calling routine is responsible for incrementing the NUM_STATES pointer.
    If there is not enough room, the size of the state list is increased.
    Zero is returned if the calling routine can now add a state; ERRNO is
    returned if ALLOCATE_STATE could not resize the state list.
*******************************************************************************/

#define  INCREMENT  4

static  int  allocate_state (

#    if __STDC__ || defined(vaxc)
        void)
#    else
        )
#    endif

{    /* Local variables. */
    char  *s ;
    int  size ;



    if (rtx->num_states >= rtx->max_states) {
        size = (rtx->max_states + INCREMENT) * sizeof (StateNode) ;
        if (rtx->state_list == NULL)
            s = malloc (size) ;
        else
            s = realloc ((char *) rtx->state_list, size) ;
        if (s == NULL) {
            vperror ("(allocate_state) Error reallocating the state list.\nrealloc: ") ;
            return (errno) ;
        }
        rtx->state_list = (StateNode *) s ;
        rtx->max_states = rtx->max_states + INCREMENT ;
    }
    return (0) ;

}

/*******************************************************************************
    Function FIRST_CHAR_OF computes the set of characters that may appear at
    the beginning of a string that could be matched by a regular expression.
    For example, the set of first characters for "abc" is simply "a"; the set
    for "[A-Za-z][A-Za-z0-9]*" contains "A" through "Z" and "a" through "z".
    The set of first characters for "a*" is the set of all characters, since
    the regular expression could still match a zero-length string if the
    target string begins with any character other than "a".  FIRST_CHAR_OF
    returns a function value of zero if the computed set does not contain
    all characters; a non-zero value is returned if the computed set does
    contain all characters.
*******************************************************************************/


static  int  first_char_of (

#    if __STDC__ || defined(vaxc)
        int  state,
        cs_set  *first_set)
#    else
        state, first_set)

        int  state ;
        cs_set  *first_set ;
#    endif

{    /* Local variables. */
    int  i ;




    if (rex_util_debug)  printf ("(first_char_of) State = %d\n", state) ;
    if ((state < 0) || (rtx->state_list[state].z.visited))  return (0) ;

/* Mark the state as visited during the traversal.  The VISITED flags keep
   the program from looping endlessly on cycles in the RE's graph (e.g.,
   closure states). */

    rtx->state_list[state].z.visited = 1 ;

/* Compute the set of first characters, starting with this state and proceeding
   to the end of the RE's graph. */

    switch (rtx->state_list[state].type) {
    case empty:
        return (first_char_of (rtx->state_list[state].next1, first_set)) ;
    case anchor:
        return (first_char_of (rtx->state_list[state].next1, first_set)) ;
    case alternation:
        if (first_char_of (rtx->state_list[state].next1, first_set))
            return (1) ;
        return (first_char_of (rtx->state_list[state].next2, first_set)) ;
    case closure:
        first_char_of (rtx->state_list[state].next2, first_set) ;
        return (first_char_of (rtx->state_list[state].next1, first_set)) ;
    case final:
        for (i = 0 ;  i < CS_SETSIZE ;  i++)
            CS_SET (i, first_set) ;
        return (1) ;
    case match:
        if (rtx->state_list[state].x.match_char < 0) {
            for (i = 0 ;  i < CS_SETSIZE ;  i++)
                CS_SET (i, first_set) ;
            return (1) ;
        } else if (rtx->state_list[state].x.match_char == 0) {
            for (i = 0 ;  i < CS_SETSIZE ;  i++) {
                if (CS_ISSET (i, rtx->state_list[state].y.match_charset))
                    CS_SET (i, first_set) ;
            }
        } else {
            CS_SET (rtx->state_list[state].x.match_char, first_set) ;
        }
        return (0) ;
    case left_paren:
        return (first_char_of (rtx->state_list[state].next1, first_set)) ;
    case right_paren:
        return (first_char_of (rtx->state_list[state].next1, first_set)) ;
    default:
        return (0) ;
    }

}

/*******************************************************************************
    Function LAST_OF locates the last state in a regular expression graph.
    LAST_OF, passed the index of the first state in the graph, simply traverses
    the NEXT1 links until a null link (index = -1) is found.  Note that this
    technique assumes that the NEXT1 links lead to the last state in the
    graph; RRPARSE adheres to this convention and uses NEXT2 links only for
    backtracking in closure (RE* and RE+) expressions.
*******************************************************************************/

static  int  last_of (

#    if __STDC__ || defined(vaxc)
        int  state)		/* The first state in the RE graph. */
#    else
        state)

        int  state ;		/* The first state in the RE graph. */
#    endif

{
    while (rtx->state_list[state].next1 >= 0)
        state = rtx->state_list[state].next1 ;
    return (state) ;
}

/*******************************************************************************
    Function LONGEST_PATH computes the longest path through an RE.  The
    longest path is an estimate of the maximum number of states that would
    be visited during an attempt to match a target string.
*******************************************************************************/


static  int  longest_path (

#    if __STDC__ || defined(vaxc)
        int  state)
#    else
        state)

        int  state ;
#    endif

{    /* Local variables. */
    int  i, j ;



    if (rex_util_debug)  printf ("(longest_path) State = %d\n", state) ;
    if ((state < 0) || (rtx->state_list[state].z.visited))  return (0) ;

/* Mark the state as visited during the traversal.  The VISITED flags keep
   the program from looping endlessly on cycles in the RE's graph (e.g.,
   closure states). */

    rtx->state_list[state].z.visited = 1 ;

/* Compute the longest path from this state to the end of the RE's graph. */

    switch (rtx->state_list[state].type) {
    case empty:
        return (longest_path (rtx->state_list[state].next1)) ;
    case anchor:
        return (longest_path (rtx->state_list[state].next1)) ;
    case alternation:
        i = 1 + longest_path (rtx->state_list[state].next1) ;
        j = 1 + longest_path (rtx->state_list[state].next2) ;
        return ((i > j) ? i : j) ;
    case closure:
        return (longest_path (rtx->state_list[state].next2) + 2 +
                longest_path (rtx->state_list[state].next1)) ;
    case final:
        return (0) ;
    case match:
        return (1 + longest_path (rtx->state_list[state].next1)) ;
    case left_paren:
        return (1 + longest_path (rtx->state_list[state].next1)) ;
    case right_paren:
        return (1 + longest_path (rtx->state_list[state].next1)) ;
    default:
        return (0) ;
    }

}

/*******************************************************************************
    Function SHORTEST_PATH computes the shortest path through an RE.  The
    shortest path equals the minimum length of a target string that would
    be matched by the RE.
*******************************************************************************/


static  int  shortest_path (

#    if __STDC__ || defined(vaxc)
        int  state)
#    else
        state)

        int  state ;
#    endif

{    /* Local variables. */
    int  i, j ;




    if (rex_util_debug)  printf ("(shortest_path) State = %d\n", state) ;
    if ((state < 0) || (rtx->state_list[state].z.visited))  return (0) ;

/* Mark the state as visited during the traversal.  The VISITED flags keep
   the program from looping endlessly on cycles in the RE's graph (e.g.,
   closure states). */

    rtx->state_list[state].z.visited = 1 ;

/* Compute the shortest path from this state to the end of the RE's graph. */

    switch (rtx->state_list[state].type) {
    case empty:
        return (shortest_path (rtx->state_list[state].next1)) ;
    case anchor:
        return (shortest_path (rtx->state_list[state].next1)) ;
    case alternation:
        i = shortest_path (rtx->state_list[state].next1) ;
        j = shortest_path (rtx->state_list[state].next2) ;
        return ((i < j) ? i : j) ;
    case closure:
        return ((shortest_path (rtx->state_list[state].next2) *
                 rtx->state_list[state].x.min_closure)  +
                shortest_path (rtx->state_list[state].next1)) ;
    case final:
        return (0) ;
    case match:
        return (1 + shortest_path (rtx->state_list[state].next1)) ;
    case left_paren:
        return (shortest_path (rtx->state_list[state].next1)) ;
    case right_paren:
        return (shortest_path (rtx->state_list[state].next1)) ;
    default:
        return (0) ;
    }

}
#line 877 "y.tab.c"
#define RRABORT goto rrabort
#define RRREJECT goto rrabort
#define RRACCEPT goto rraccept
#define RRERROR goto rrerrlab
int
rrparse()
{
    register int rrm, rrn, rrstate;
#if RRDEBUG
    register char *rrs;
    extern char *getenv();

    if (rrs = getenv("RRDEBUG"))
    {
        rrn = *rrs;
        if (rrn >= '0' && rrn <= '9')
            rrdebug = rrn - '0';
    }
#endif

    rrnerrs = 0;
    rrerrflag = 0;
    rrchar = (-1);

    rrssp = rrss;
    rrvsp = rrvs;
    *rrssp = rrstate = 0;

rrloop:
    if (rrn = rrdefred[rrstate]) goto rrreduce;
    if (rrchar < 0)
    {
        if ((rrchar = rrlex()) < 0) rrchar = 0;
#if RRDEBUG
        if (rrdebug)
        {
            rrs = 0;
            if (rrchar <= RRMAXTOKEN) rrs = rrname[rrchar];
            if (!rrs) rrs = "illegal-symbol";
            printf("%sdebug: state %d, reading %d (%s)\n",
                    RRPREFIX, rrstate, rrchar, rrs);
        }
#endif
    }
    if ((rrn = rrsindex[rrstate]) && (rrn += rrchar) >= 0 &&
            rrn <= RRTABLESIZE && rrcheck[rrn] == rrchar)
    {
#if RRDEBUG
        if (rrdebug)
            printf("%sdebug: state %d, shifting to state %d\n",
                    RRPREFIX, rrstate, rrtable[rrn]);
#endif
        if (rrssp >= rrss + rrstacksize - 1)
        {
            goto rroverflow;
        }
        *++rrssp = rrstate = rrtable[rrn];
        *++rrvsp = rrlval;
        rrchar = (-1);
        if (rrerrflag > 0)  --rrerrflag;
        goto rrloop;
    }
    if ((rrn = rrrindex[rrstate]) && (rrn += rrchar) >= 0 &&
            rrn <= RRTABLESIZE && rrcheck[rrn] == rrchar)
    {
        rrn = rrtable[rrn];
        goto rrreduce;
    }
    if (rrerrflag) goto rrinrecovery;
#ifdef lint
    goto rrnewerror;
#endif
rrnewerror:
    rrerror("syntax error");
#ifdef lint
    goto rrerrlab;
#endif
rrerrlab:
    ++rrnerrs;
rrinrecovery:
    if (rrerrflag < 3)
    {
        rrerrflag = 3;
        for (;;)
        {
            if ((rrn = rrsindex[*rrssp]) && (rrn += RRERRCODE) >= 0 &&
                    rrn <= RRTABLESIZE && rrcheck[rrn] == RRERRCODE)
            {
#if RRDEBUG
                if (rrdebug)
                    printf("%sdebug: state %d, error recovery shifting\
 to state %d\n", RRPREFIX, *rrssp, rrtable[rrn]);
#endif
                if (rrssp >= rrss + rrstacksize - 1)
                {
                    goto rroverflow;
                }
                *++rrssp = rrstate = rrtable[rrn];
                *++rrvsp = rrlval;
                goto rrloop;
            }
            else
            {
#if RRDEBUG
                if (rrdebug)
                    printf("%sdebug: error recovery discarding state %d\n",
                            RRPREFIX, *rrssp);
#endif
                if (rrssp <= rrss) goto rrabort;
                --rrssp;
                --rrvsp;
            }
        }
    }
    else
    {
        if (rrchar == 0) goto rrabort;
#if RRDEBUG
        if (rrdebug)
        {
            rrs = 0;
            if (rrchar <= RRMAXTOKEN) rrs = rrname[rrchar];
            if (!rrs) rrs = "illegal-symbol";
            printf("%sdebug: state %d, error recovery discards token %d (%s)\n",
                    RRPREFIX, rrstate, rrchar, rrs);
        }
#endif
        rrchar = (-1);
        goto rrloop;
    }
rrreduce:
#if RRDEBUG
    if (rrdebug)
        printf("%sdebug: state %d, reducing by rule %d (%s)\n",
                RRPREFIX, rrstate, rrn, rrrule[rrn]);
#endif
    rrm = rrlen[rrn];
    rrval = rrvsp[1-rrm];
    switch (rrn)
    {
case 1:
#line 249 "rex_util_y.y"
{ display_production ("complete_re", "regular_expression") ;
            /* The initial state equals the first state in RE.  Add a final
               state and link the last state in RE to the final state. */
            rtx->start_state = rrvsp[0] ;
            NEW_STATE ;
            rtx->state_list[last_of (rrvsp[0])].next1 = rtx->num_states ;
            rtx->state_list[rtx->num_states].type = final ;
            rtx->state_list[rtx->num_states].x.match_char = ' ' ;
            rtx->num_states++ ;
            /* Estimate the length of the longest path through the complete
               RE.  The length is used to size the stack in the iterative
               version of REX_SEARCH. */
            for (i = 0 ;  i < rtx->num_states ;  i++)
                rtx->state_list[i].z.visited = 0 ;
            rtx->longest_path = longest_path (rrvsp[0]) ;
            /* Compute the set of first characters that may appear at the
               beginning of a target string matched by this RE.  This set
               allows REX_MATCH to avoid calling REX_SEARCH when the first
               character of the target string is not in the set of first
               characters. */
            CS_ZERO (&rtx->first_set) ;
            for (i = 0 ;  i < rtx->num_states ;  i++)
                rtx->state_list[i].z.visited = 0 ;
            first_char_of (rtx->start_state, &rtx->first_set) ;
          }
break;
case 2:
#line 279 "rex_util_y.y"
{ display_production ("regular_expression", "<empty>") ;
            /* Add an EMPTY state. */
            NEW_STATE ;
            rtx->state_list[rtx->num_states].type = empty ;
            rtx->state_list[rtx->num_states].x.match_char = ' ' ;
            rtx->state_list[rtx->num_states].next1 = -1 ;
            rtx->state_list[rtx->num_states].next2 = -1 ;
            rrval = rtx->num_states++ ;
          }
break;
case 3:
#line 289 "rex_util_y.y"
{ display_production ("regular_expression", "term") ;  rrval = rrvsp[0] ; }
break;
case 4:
#line 291 "rex_util_y.y"
{ display_production ("regular_expression", "regular_expression | regular_expression") ;
            /* Add an alternation state E1 and an empty state E2.  Link E1 to
               the first state in RE1 and the first state in RE2.  Link the
               last state in RE1 to E2; do the same for the last state in RE2.
               (The first state in "RE|RE" is E1.) */
            NEW_STATE ;
            rtx->state_list[rtx->num_states].type = alternation ;
            rtx->state_list[rtx->num_states].x.match_char = ' ' ;
            rtx->state_list[rtx->num_states].next1 = rrvsp[-2] ;
            rtx->state_list[rtx->num_states].next2 = rrvsp[0] ;
            rrval = rtx->num_states++ ;
            NEW_STATE ;
            rtx->state_list[last_of (rrvsp[-2])].next1 = rtx->num_states ;
            rtx->state_list[last_of (rrvsp[0])].next1 = rtx->num_states ;
            rtx->state_list[rtx->num_states].type = empty ;
            rtx->state_list[rtx->num_states].x.match_char = ' ' ;
            rtx->state_list[rtx->num_states].next1 = -1 ;
            rtx->state_list[rtx->num_states].next2 = -1 ;
            rtx->num_states++ ;
          }
break;
case 5:
#line 316 "rex_util_y.y"
{ display_production ("term", "factor") ;  rrval = rrvsp[0] ; }
break;
case 6:
#line 318 "rex_util_y.y"
{ display_production ("term", "factor term") ;
            /* Link the last state in RE1 to the first state in RE2.  (The
               first state in "RE1 RE2" is the first state in RE1.) */
            rtx->state_list[last_of (rrvsp[-1])].next1 = rrvsp[0] ;
            rrval = rrvsp[-1] ;
          }
break;
case 7:
#line 329 "rex_util_y.y"
{ display_production ("factor", "^") ;
            /* Add an anchor state and set its match character to "^". */
            NEW_STATE ;
            rtx->state_list[rtx->num_states].type = anchor ;
            rtx->state_list[rtx->num_states].x.match_char = '^' ;
            rtx->state_list[rtx->num_states].next1 = -1 ;
            rtx->state_list[rtx->num_states].next2 = -1 ;
            rrval = rtx->num_states++ ;
          }
break;
case 8:
#line 339 "rex_util_y.y"
{ display_production ("factor", "$") ;
            /* Add an anchor state and set its match character to "$". */
            NEW_STATE ;
            rtx->state_list[rtx->num_states].type = anchor ;
            rtx->state_list[rtx->num_states].x.match_char = '$' ;
            rtx->state_list[rtx->num_states].next1 = -1 ;
            rtx->state_list[rtx->num_states].next2 = -1 ;
            rrval = rtx->num_states++ ;
          }
break;
case 9:
#line 349 "rex_util_y.y"
{ display_production ("factor", "factor{[min][,[max]]}") ;
            /* Check for nested closures that might cause the matching
               algorithm to endlessly loop without consuming any input
               from the target string; e.g., "(a*){0,10}".  Actually,
               the maximum field puts a cap on the number of loops, but
               we'll disallow it anyway. */
            for (i = 0 ;  i < rtx->num_states ;  i++)
                rtx->state_list[i].z.visited = 0 ;
            i = shortest_path (rrvsp[-4]) ;
            if (rex_util_debug)  printf ("(rrparse) Shortest path from state %d = %d\n", rrvsp[-4], i) ;
            if (i <= 0) {
                errno = EINVAL ;  rex_error_text = "nested empty closure" ;
                return (errno) ;
            }
            /* Add a closure state and set its minimum and maximum fields.
               Link the last state in RE to the closure state; link the closure
               state backwards to the first state in RE.  (The first state in
               "RE{min,max}" is the closure state.) */
            NEW_STATE ;
            rtx->state_list[last_of (rrvsp[-4])].next1 = rtx->num_states ;
            rtx->state_list[rtx->num_states].type = closure ;
            if (rrvsp[-2] < 0)  rrvsp[-2] = 0 ;
            rtx->state_list[rtx->num_states].x.min_closure = rrvsp[-2] ;
            if (rrvsp[-1] < 0)  rrvsp[-1] = rrvsp[-2] ;
            rtx->state_list[rtx->num_states].y.max_closure = rrvsp[-1] ;
            rtx->state_list[rtx->num_states].next1 = -1 ;
            rtx->state_list[rtx->num_states].next2 = rrvsp[-4] ;
            rrval = rtx->num_states++ ;
          }
break;
case 10:
#line 379 "rex_util_y.y"
{ rrparse_expecting_assignment = -1 ; }
break;
case 11:
#line 381 "rex_util_y.y"
{ display_production ("factor", "(regular_expression) <assignment>") ;
            /* If an assignment clause ("$<digit>") was specified, add two
               parenthesis states, P1 and P2.  Link P1 to the first state
               in RE; link the last state in RE to P2.  Store the argument
               index (<digit>) in P1 and store the index of P1 in P2.  If an
               assignment clause was NOT specified, nothing needs to be done.
               (The first state in "(RE)" is the first state in RE; the first
                state in "(RE)$n" is P1.) */
            if (rrvsp[0] < 0) {				/* "(RE)" */
                rrval = rrvsp[-3] ;
            } else {					/* "(RE)$n" */
                NEW_STATE ;
                rtx->state_list[rtx->num_states].type = left_paren ;	/* P1 */
                rtx->state_list[rtx->num_states].x.subexp_index = rrvsp[0] ;
                rtx->state_list[rtx->num_states].y.subexp_state =
                    rtx->num_states + 1 ;
                rtx->state_list[rtx->num_states].next1 = rrvsp[-3] ;
                rtx->state_list[rtx->num_states].next2 = -1 ;
                if (rtx->num_args < (rrvsp[0]+1))  rtx->num_args = rrvsp[0] + 1 ;
                rrval = rtx->num_states++ ;
                NEW_STATE ;
                rtx->state_list[last_of (rrvsp[-3])].next1 = rtx->num_states ;
                rtx->state_list[rtx->num_states].type = right_paren ;	/* P2 */
                rtx->state_list[rtx->num_states].x.subexp_index = rrvsp[0] ;
                rtx->state_list[rtx->num_states].y.subexp_state = rrval ;
                rtx->state_list[rtx->num_states].next1 = -1 ;
                rtx->state_list[rtx->num_states].next2 = -1 ;
                rtx->num_states++ ;
            }
          }
break;
case 12:
#line 412 "rex_util_y.y"
{ display_production ("factor", "factor*") ;
            /* Check for nested closures that might cause the matching
               algorithm to endlessly loop without consuming any input
               from the target string; e.g., "(a*)*". */
            for (i = 0 ;  i < rtx->num_states ;  i++)
                rtx->state_list[i].z.visited = 0 ;
            i = shortest_path (rrvsp[-1]) ;
            if (rex_util_debug)  printf ("(rrparse) Shortest path from state %d = %d\n", rrvsp[-1], i) ;
            if (i <= 0) {
                errno = EINVAL ;  rex_error_text = "nested empty closure" ;
                return (errno) ;
            }
            /* Add a closure state.  Link the last state in RE to the closure
               state; link the closure state backwards to the first state in
               RE.  (The first state in "RE*" is the closure state.) */
            NEW_STATE ;
            rtx->state_list[last_of (rrvsp[-1])].next1 = rtx->num_states ;
            rtx->state_list[rtx->num_states].type = closure ;
            rtx->state_list[rtx->num_states].x.min_closure = 0 ;
            rtx->state_list[rtx->num_states].y.max_closure = INT_MAX ;
            rtx->state_list[rtx->num_states].next1 = -1 ;
            rtx->state_list[rtx->num_states].next2 = rrvsp[-1] ;
            rrval = rtx->num_states++ ;
          }
break;
case 13:
#line 437 "rex_util_y.y"
{ display_production ("factor", "factor+") ;
            /* Check for nested closures that might cause the matching
               algorithm to endlessly loop without consuming any input
               from the target string; e.g., "(a*)+". */
            for (i = 0 ;  i < rtx->num_states ;  i++)
                rtx->state_list[i].z.visited = 0 ;
            i = shortest_path (rrvsp[-1]) ;
            if (rex_util_debug)  printf ("(rrparse) Shortest path from state %d = %d\n", rrvsp[-1], i) ;
            if (i <= 0) {
                errno = EINVAL ;  rex_error_text = "nested empty closure" ;
                return (errno) ;
            }
            /* Add a closure state.  Link the last state in RE to the closure
               state; link the closure state backwards to the first state in
               RE.  (The first state in "RE+" is the closure state.) */
            NEW_STATE ;
            rtx->state_list[last_of (rrvsp[-1])].next1 = rtx->num_states ;
            rtx->state_list[rtx->num_states].type = closure ;
            rtx->state_list[rtx->num_states].x.min_closure = 1 ;
            rtx->state_list[rtx->num_states].y.max_closure = INT_MAX ;
            rtx->state_list[rtx->num_states].next1 = -1 ;
            rtx->state_list[rtx->num_states].next2 = rrvsp[-1] ;
            rrval = rtx->num_states++ ;
          }
break;
case 14:
#line 462 "rex_util_y.y"
{ display_production ("factor", "factor?") ;
            /* Check for nested closures.  Even though nested closures are
               not a major threat in a zero-or-one closure, we check anyway,
               just to be consistent with the other forms of closure. */
            for (i = 0 ;  i < rtx->num_states ;  i++)
                rtx->state_list[i].z.visited = 0 ;
            i = shortest_path (rrvsp[-1]) ;
            if (rex_util_debug)  printf ("(rrparse) Shortest path from state %d = %d\n", rrvsp[-1], i) ;
            if (i <= 0) {
                errno = EINVAL ;  rex_error_text = "nested empty closure" ;
                return (errno) ;
            }
            /* Add a closure state.  Link the last state in RE to the closure
               state; link the closure state backwards to the first state in
               RE.  (The first state in "RE?" is the closure state.) */
            NEW_STATE ;
            rtx->state_list[last_of (rrvsp[-1])].next1 = rtx->num_states ;
            rtx->state_list[rtx->num_states].type = closure ;
            rtx->state_list[rtx->num_states].x.min_closure = 0 ;
            rtx->state_list[rtx->num_states].y.max_closure = 1 ;
            rtx->state_list[rtx->num_states].next1 = -1 ;
            rtx->state_list[rtx->num_states].next2 = rrvsp[-1] ;
            rrval = rtx->num_states++ ;
          }
break;
case 15:
#line 487 "rex_util_y.y"
{ CS_ZERO (&char_class) ;
            rrparse_after_bracket = 1 ;
            rrparse_hyphen_as_literal = 0 ;
          }
break;
case 16:
#line 492 "rex_util_y.y"
{ display_production ("factor", "[character_classes]") ;
            rrparse_after_bracket = 0 ;
            if (rrvsp[-2]) {
                for (i = 0 ;  i < CS_SETSIZE ;  i++) {
                    if (CS_ISSET (i, &char_class))
                        CS_CLR (i, &char_class) ;
                    else
                        CS_SET (i, &char_class) ;
                }
            }
            if (rex_util_debug) {
                printf ("Character Class:\n") ;
                rex_dump_class (stdout, "    Matches: ", &char_class) ;
            }
            /* Add a multi-character state for the character class. */
            NEW_STATE ;
            rtx->state_list[rtx->num_states].type = match ;
            rtx->state_list[rtx->num_states].x.match_char = 0 ;
            rtx->state_list[rtx->num_states].y.match_charset =
                (cs_set *) malloc (sizeof (cs_set)) ;
            if (rtx->state_list[rtx->num_states].y.match_charset == NULL) {
                vperror ("(rrparse) Error allocating character class set.\nmalloc: ") ;
                rex_error_text = "error allocating character class set" ;
                return (errno) ;
            }
            CS_ZERO (rtx->state_list[rtx->num_states].y.match_charset) ;
            for (i = 0 ;  i < CS_SETSIZE ;  i++)
                if (CS_ISSET (i, &char_class))
                    CS_SET (i, rtx->state_list[rtx->num_states].y.match_charset) ;
            rtx->state_list[rtx->num_states].next1 = -1 ;
            rtx->state_list[rtx->num_states].next2 = -1 ;
            rrval = rtx->num_states++ ;
          }
break;
case 17:
#line 526 "rex_util_y.y"
{ display_production ("factor", "SINGLE_CHARACTER") ;
            /* Add a one-character state for the character. */
            NEW_STATE ;
            rtx->state_list[rtx->num_states].type = match ;
            rtx->state_list[rtx->num_states].x.match_char = rrvsp[0] ;
            rtx->state_list[rtx->num_states].next1 = -1 ;
            rtx->state_list[rtx->num_states].next2 = -1 ;
            rrval = rtx->num_states++ ;
          }
break;
case 18:
#line 536 "rex_util_y.y"
{ display_production ("factor", "COMMA") ;
            /* Add a one-character state for ",". */
            NEW_STATE ;
            rtx->state_list[rtx->num_states].type = match ;
            rtx->state_list[rtx->num_states].x.match_char = rrvsp[0] ;
            rtx->state_list[rtx->num_states].next1 = -1 ;
            rtx->state_list[rtx->num_states].next2 = -1 ;
            rrval = rtx->num_states++ ;
          }
break;
case 19:
#line 546 "rex_util_y.y"
{ display_production ("factor", "DIGIT") ;
            /* Add a one-character state for the digit. */
            NEW_STATE ;
            rtx->state_list[rtx->num_states].type = match ;
            rtx->state_list[rtx->num_states].x.match_char = rrvsp[0] ;
            rtx->state_list[rtx->num_states].next1 = -1 ;
            rtx->state_list[rtx->num_states].next2 = -1 ;
            rrval = rtx->num_states++ ;
          }
break;
case 20:
#line 556 "rex_util_y.y"
{ display_production ("factor", "ANY_CHARACTER") ;
            /* Add a one-character state for any-character-matches. */
            NEW_STATE ;
            rtx->state_list[rtx->num_states].type = match ;
            rtx->state_list[rtx->num_states].x.match_char = -1 ;
            rtx->state_list[rtx->num_states].next1 = -1 ;
            rtx->state_list[rtx->num_states].next2 = -1 ;
            rrval = rtx->num_states++ ;
          }
break;
case 21:
#line 570 "rex_util_y.y"
{ display_production ("assignment", "<empty>") ;  rrval = -1 ; }
break;
case 22:
#line 572 "rex_util_y.y"
{ display_production ("assignment", "$<0-9>") ;
            sprintf (buffer, "%c", rrvsp[0]) ;  rrval = atoi (buffer) ;
          }
break;
case 23:
#line 580 "rex_util_y.y"
{ display_production ("complement", "<empty>") ;  rrval = 0 ; }
break;
case 24:
#line 582 "rex_util_y.y"
{ display_production ("complement", "^") ;  rrval = -1 ; }
break;
case 25:
#line 588 "rex_util_y.y"
{ display_production ("number", "<empty>") ;  rrval = -1 ; }
break;
case 26:
#line 590 "rex_util_y.y"
{ display_production ("number", "digits") ;  rrval = rrvsp[0] ; }
break;
case 27:
#line 596 "rex_util_y.y"
{ display_production ("upper_bound", "<empty>") ;
            rrval = -1 ;			/* Upper bound = lower bound. */
          }
break;
case 28:
#line 600 "rex_util_y.y"
{ display_production ("lower_bound", "digits") ;
            if (rrvsp[0] < 0)
                rrval = INT_MAX ;		/* Upper bound = infinity. */
            else
                rrval = rrvsp[0] ;		/* Normal upper bound. */
          }
break;
case 29:
#line 611 "rex_util_y.y"
{ display_production ("digits", "DIGIT") ;
            sprintf (buffer, "%c", rrvsp[0]) ;  rrval = atoi (buffer) ;
          }
break;
case 30:
#line 615 "rex_util_y.y"
{ display_production ("digits", "digits DIGIT") ;
            sprintf (buffer, "%c", rrvsp[0]) ;  rrval = (rrvsp[-1] * 10) + atoi (buffer) ;
          }
break;
case 31:
#line 623 "rex_util_y.y"
{ display_production ("character_classes", "character_class") ; }
break;
case 32:
#line 625 "rex_util_y.y"
{ display_production ("character_classes", "character_classes character_class") ; }
break;
case 33:
#line 631 "rex_util_y.y"
{ display_production ("character_class", "character") ;
            CS_SET (rrvsp[0], &char_class) ;
          }
break;
case 34:
#line 635 "rex_util_y.y"
{ rrparse_hyphen_as_literal = 1 ; }
break;
case 35:
#line 637 "rex_util_y.y"
{ display_production ("character_class", "character-character") ;
            rrparse_hyphen_as_literal = 0 ;
            if (rrvsp[-3] > rrvsp[0]) {
                if (rex_util_debug)  printf ("(rrparse) Start character exceeds end character in character class range \"%c-%c\".\n", rrvsp[-3], rrvsp[0]) ;
                errno = EINVAL ;
                rex_error_text = "inverted character class range" ;
                return (errno) ;
            }
            for (i = rrvsp[-3] ;  i <= rrvsp[0] ;  i++)
                CS_SET (i, &char_class) ;
          }
break;
case 36:
#line 649 "rex_util_y.y"
{ display_production ("character_class", ":<class>:") ;
            for (i = 0 ;  i < CS_SETSIZE ;  i++) {
                switch (rrvsp[0]) {
                case  0:  if (isalnum (i))  CS_SET (i, &char_class) ;  break ;
                case  1:  if (isalpha (i))  CS_SET (i, &char_class) ;  break ;
                case  2:  if (iscntrl (i))  CS_SET (i, &char_class) ;  break ;
                case  3:  if (isdigit (i))  CS_SET (i, &char_class) ;  break ;
                case  4:  if (isgraph (i))  CS_SET (i, &char_class) ;  break ;
                case  5:  if (islower (i))  CS_SET (i, &char_class) ;  break ;
                case  6:  if (isprint (i))  CS_SET (i, &char_class) ;  break ;
                case  7:  if (ispunct (i))  CS_SET (i, &char_class) ;  break ;
                case  8:  if (isspace (i))  CS_SET (i, &char_class) ;  break ;
                case  9:  if (isupper (i))  CS_SET (i, &char_class) ;  break ;
                case 10:  if (isxdigit (i)) CS_SET (i, &char_class) ;  break ;
                }
            }
          }
break;
case 37:
#line 671 "rex_util_y.y"
{ display_production ("character", "SINGLE_CHARACTER") ;  rrval = rrvsp[0] ; }
break;
#line 1472 "y.tab.c"
    }
    rrssp -= rrm;
    rrstate = *rrssp;
    rrvsp -= rrm;
    rrm = rrlhs[rrn];
    if (rrstate == 0 && rrm == 0)
    {
#if RRDEBUG
        if (rrdebug)
            printf("%sdebug: after reduction, shifting from state 0 to\
 state %d\n", RRPREFIX, RRFINAL);
#endif
        rrstate = RRFINAL;
        *++rrssp = RRFINAL;
        *++rrvsp = rrval;
        if (rrchar < 0)
        {
            if ((rrchar = rrlex()) < 0) rrchar = 0;
#if RRDEBUG
            if (rrdebug)
            {
                rrs = 0;
                if (rrchar <= RRMAXTOKEN) rrs = rrname[rrchar];
                if (!rrs) rrs = "illegal-symbol";
                printf("%sdebug: state %d, reading %d (%s)\n",
                        RRPREFIX, RRFINAL, rrchar, rrs);
            }
#endif
        }
        if (rrchar == 0) goto rraccept;
        goto rrloop;
    }
    if ((rrn = rrgindex[rrm]) && (rrn += rrstate) >= 0 &&
            rrn <= RRTABLESIZE && rrcheck[rrn] == rrstate)
        rrstate = rrtable[rrn];
    else
        rrstate = rrdgoto[rrm];
#if RRDEBUG
    if (rrdebug)
        printf("%sdebug: after reduction, shifting from state %d \
to state %d\n", RRPREFIX, *rrssp, rrstate);
#endif
    if (rrssp >= rrss + rrstacksize - 1)
    {
        goto rroverflow;
    }
    *++rrssp = rrstate;
    *++rrvsp = rrval;
    goto rrloop;
rroverflow:
    rrerror("yacc stack overflow");
rrabort:
    return (1);
rraccept:
    return (0);
}
