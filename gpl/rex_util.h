/*
@(#)  FILE: rex_util.h  RELEASE: 1.6  DATE: 11/14/97, 16:42:59
*/
/*******************************************************************************

    rex_util.h

    Regular Expression Utility Definitions.

*******************************************************************************/

#ifndef  REX_UTIL_H		/* Has the file been INCLUDE'd already? */
#define  REX_UTIL_H  yes

#ifdef __cplusplus
    extern  "C" {
#endif


#include  "ansi_setup.h"		/* ANSI or non-ANSI C? */


/*******************************************************************************
    Compiled Regular Expressions (Client View).
*******************************************************************************/

typedef  struct  _CompiledRE  *CompiledRE ;


/*******************************************************************************
    Miscellaneous declarations.
*******************************************************************************/

extern  int  rex_util_debug ;		/* Global debug switch (1/0 = yes/no). */
extern  char  *rex_error_text ;		/* Message to pinpoint error during parse. */


/*******************************************************************************
    Public functions.
*******************************************************************************/

extern  int  rex_compile P_((const char *expression,
                             CompiledRE *pattern)) ;

extern  int  rex_delete P_((CompiledRE pattern)) ;

extern  int  rex_dump P_((FILE *outfile, CompiledRE pattern)) ;

extern  int  rex_match P_((const char *target,
                           CompiledRE pattern,
                           char **matchStart,
                           int *matchLength,
                           int numSubExps,
                           ...)) ;

extern  int  rex_nosex P_((CompiledRE pattern)) ;

extern  int  rex_replace P_((const char *source,
                             CompiledRE pattern,
                             const char *replacement,
                             int maxSubstitutions,
                             char **result,
                             int *numSubstitutions)) ;

extern  char  *rex_wild P_((const char *wildcard)) ;


#ifdef __cplusplus
    }
#endif

#endif				/* If this file was not INCLUDE'd previously. */
