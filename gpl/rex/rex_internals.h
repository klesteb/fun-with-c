/*
@(#)  FILE: rex_internals.h  RELEASE: 1.3  DATE: 10/14/97, 13:13:02
*/
/*******************************************************************************

  rex_internals.h

  Regular Expression Utility Internal Definitions.

*******************************************************************************/


#ifndef  REX_INTERNALS_H	/* Has the file been INCLUDE'd already? */
#define  REX_INTERNALS_H  yes


#include  "gpl/rex_util.h"	/* Regular expression utility definitions. */


#ifndef P_
#    if __STDC__ || defined(__cplusplus) || defined(vaxc)
#        define  P_(s)  s
#    else
#        define  P_(s)  ()
#        define  const
#    endif
#endif


/*******************************************************************************
    The following section of the INCLUDE file defines macro's for manipulating
    character set bit masks.  The macro definitions are modelled after the file
    descriptor bit mask macro's (FD_SET, FD_CLR, etc.), defined in either the
    system INCLUDE file, TYPES.H, or our local INCLUDE file, FD.H.
*******************************************************************************/

#ifdef VXWORKS
#    include <types.h>
#    define  memset(s,c,n)  bfill(s,n,c)
#else
#    include  <sys/types.h>		/* System type definitions. */
#endif

#ifndef NBBY
#define	NBBY	8		/* number of bits in a byte */
#endif

#define	CS_SETSIZE	256 

/* #define	CS_SETSIZE	128 */
/* typedef int	cs_mask; */

typedef	long	cs_mask;
#define	NCSBITS	(sizeof(cs_mask) * NBBY)	/* bits per mask */
#ifndef	howmany
#ifdef sun386
#define howmany(x, y)   ((((u_int)(x))+(((u_int)(y))-1))/((u_int)(y)))
#else
#define	howmany(x, y)	(((x)+((y)-1))/(y))
#endif
#endif

typedef	struct cs_set {
	cs_mask	css_bits[howmany(CS_SETSIZE, NCSBITS)];
} cs_set;

#ifdef __VMS
int CS_SET(int n, struct cs_set *p);
int CS_CLR(int n, struct cs_set *p);
int CS_ISSET(int n, struct cs_set *p);
#else
#define	CS_SET(n, p)	((p)->css_bits[(n)/NCSBITS] |= (1 << ((n) % NCSBITS)))
#define	CS_CLR(n, p)	((p)->css_bits[(n)/NCSBITS] &= ~(1 << ((n) % NCSBITS)))
#define	CS_ISSET(n, p)	((p)->css_bits[(n)/NCSBITS] & (1 << ((n) % NCSBITS)))
#endif
#define	CS_ZERO(p)	memset ((char *)(p), '\0', sizeof(*(p)))


/*******************************************************************************

    Regular Expression Graph
    ------------------------

    A compiled regular expression is represented by a context node that
    contains a pointer to an array of state nodes:

                                .---------.
        Compiled Pattern -----> | Context |
                                |   Node  |     .---------.
                                |    *--------->| State 0 |
                                `---------'     | State 1 |
                                                |   ...   |
                                                |   ...   |
                                                | State N |
                                                `---------'

    Each state node contains two links to other states.  These "pointers"
    are simply the indices of other states in the state array; a null
    link is indicated by an index of -1.

    State nodes are of several types: ALTERNATION, ANCHOR, CLOSURE, EMPTY,
    FINAL, LEFT_PAREN, MATCH, and RIGHT_PAREN.  An RE that matches a single
    character (e.g., "a" or "[A-Za-z]") is represented by a single MATCH
    node:

                             ----> MATCH ---->

    A MATCH node for a range of characters ("[A-Za-z]" for example) contains
    a pointer to a 128-bit bit map in which bits corresponding to in-range
    characters are set.

    The various forms of closure ("RE*", "RE+", "RE?", and "RE{m,n}")
    are represented by a CLOSURE node that is linked to the RE's graph:

               ------------------------------.
                                             V
                   .-->  RE's graph ----> CLOSURE ---->
                   |                         |
                   `-------------------------'

    The CLOSURE node contains the lower and upper bounds (M and N) for
    the closure.

    Alternating expressions (e.g., "abc|def") are formed by creating an
    ALTERNATION node with links to the two REs; the two REs are, in turn,
    linked to an EMPTY node:

                    .----------> RE1's graph -----------.
                    |                                   V
         ----> ALTERNATION                            EMPTY ----->
                    |                                   ^
                    `----------> RE2's graph -----------'

    The LEFT_PAREN and RIGHT_PAREN nodes are used for subexpression
    assignments (e.g., "(RE)$n"):

         ----> LEFT_PAREN ----> RE's graph ----> RIGHT_PAREN ---->

    The LEFT_PAREN node contains the index N (0..9) of the argument to be
    assigned the subexpression's text.  The RIGHT_PAREN node has a "hidden"
    link back to the matching LEFT_PAREN.  Parentheses used simply for
    grouping are not explicitly represented in the graph.

    Anchored regular expressions ("^RE" and "RE$") are represented by ANCHOR
    nodes:

                  ----> ANCHOR(^) ----> RE's graph ---->
                  ----> RE's graph ----> ANCHOR($) ---->

    A successful match of a target string by a regular expression occurs
    when the FINAL state of the RE's graph is reached:

                                ----> FINAL

    State nodes have the following fields:

        TYPE - the type of the state.
        MATCH_CHAR - is the character to be matched: -1 = any character but
            newline; 0 = use the MATCH_CHARSET bit mask; > 1 = the character
            to be matched.
        MATCH_CHARSET - if MATCH_CHAR equals 0, MATCH_CHARSET points to a
            dynamically-allocated character set bit mask in which the bits
            corresponding to characters that can be matched are set.
        NEXT1 - is the index of the next state, in the case of concatenated
            RE's or alternating RE's.  (A null link is indicated by an index
            of -1.)
        NEXT2 - is the index of the next state, in the case of backtracking
            closure RE's (RE* and RE+) and alternating RE's.  (A null link is
            indicated by an index of -1.)
        VISITED - indicates whether or not the state has been visited during
            a traversal.  This field is used by functions LONGEST_PATH and
            SHORTEST_PATH during the parsing of an RE.

    For closure nodes, certain fields have the following meanings:

        TYPE - is "closure".
        MIN_CLOSURE - is the minimum number of occurrences for the RE subject
            to the closure.
        MAX_CLOSURE - is the maximum number of occurrences for the RE subject
            to the closure.
        COUNT - counts the number of occurrences of an RE, while cycling
            through a closure expression during an attempt to match a pattern.

    For parentheses nodes, certain fields have the following meanings:

        TYPE - is "left_paren" or "right_paren".
        SUBEXP_INDEX - is the index (0..9) of the argument to be assigned the
            value of the matched text.
        SUBEXP_STATE - for the right parenthesis, is the index of the state
            for the corresponding left parenthesis.

    For anchor nodes, certain fields have the following meanings:

        TYPE - is "anchor".
        MATCH_CHAR - is '^' or '$', depending on what type of anchor it is.

*******************************************************************************/

    typedef  enum  StateType {
        empty, alternation, anchor, closure,
        final, left_paren, match, right_paren
    }  StateType ;

    typedef  struct  StateNode {
        StateType  type ;		/* (See member descriptions above.) */
        union {
            int  match_char ;
            int  min_closure ;
            int  subexp_index ;
        }  x ;
        union {
            cs_set  *match_charset ;
            int  max_closure ;
            int  subexp_state ;
        }  y ;
        union {
            int  count ;
            int  visited ;
        }  z ;
        int  next1, next2 ;
    }  StateNode ;


/*******************************************************************************

    Context nodes contain the context of a regular expression pattern.  The
    fields in a context node have the following meanings:

        EXPRESSION - is the text of the regular expression that was compiled.

        SCAN - is a pointer used by RRLEX to scan PATTERN during the parse of
            the regular expression.

        BOL_ANCHOR - indicates if the start state of the regular expression
            is a beginning-of-line anchor.  If this flag is TRUE, REX_MATCH
            only needs to call REX_SEARCH once, at the beginning of the target
            string; it does NOT need to shift the starting point for REX_SEARCH
            through each character in the target string

        NUM_ARGS - is the number of argument clauses ("(RE)$n") found in the
            regular expression.  Application programs can retrieve this value
            by calling REX_NOSEX; interactive programs that input an RE at
            run-time may find this information useful.

        LONGEST_PATH - is the number of nodes along the longest path through
            a regular expression.  This length is used to estimate the size
            of the stack required by the iterative version of REX_SEARCH.

        FIRST_SET - is a bit map in which set bits correspond to characters
            that may appear as the first character in a target string matched
            by the regular expression.  REX_MATCH uses this information to
            skip calls to REX_SEARCH when the first character in a target
            string admits no possibility of a match.

        MAX_STATES - is the maximum number of state entries in the context
            node's state list.  RRPARSE increments this as needed as the
            regular expression is parsed.

        NUM_STATES - is the actual number of state entries in the context
            node's state list.

        START_STATE - is the index in the state list of the initial state of
            the regular expression's graph.

        STATE_LIST - points to a dynamically-allocated array of state nodes.

*******************************************************************************/

    typedef  struct  _CompiledRE {
        char  *expression ;		/* (See member descriptions above.) */
        char  *scan ;
        int  bol_anchor ;
        int  num_args ;
        int  longest_path ;
        cs_set  first_set ;
        int  max_states ;
        int  num_states ;
        int  start_state ;
        StateNode  *state_list ;	/* Pointer to array of state nodes. */
    }  _CompiledRE ;


    extern  CompiledRE  rtx ;		/* Regular expression being parsed. */


#define  RRSTYPE  int


/*******************************************************************************
    Public functions.
*******************************************************************************/

/* REX_UTIL.C */
extern  int  rex_dump_class P_((FILE *outfile,
                                const char *lead_in,
                                cs_set *char_set)) ;

/* REX_UTIL_Y.Y */
extern  int  rrparse P_((void)) ;


#endif				/* If this file was not INCLUDE'd previously. */

