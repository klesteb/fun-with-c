/* +++Date last modified: 05-Jul-1997 */

/************************************************************************/
/*                                                                      */
/*  MFLFILES Header file                                                */
/*                                                                      */
/*  Function prototypes for file system access functions.               */
/*                                                                      */
/*  Original Copyright 1990-96 by Robert B. Stout as part of            */
/*  the MicroFirm Function Library (MFL)                                */
/*                                                                      */
/*  The user is granted a free limited license to use this source file  */
/*  to create royalty-free programs, subject to the terms of the        */
/*  license restrictions specified in the LICENSE.MFL file.             */
/*                                                                      */
/************************************************************************/

#ifndef GETOPTS_H
#define GETOPTS_H

#include <stdio.h>
#include "sniptype.h"
#include "extkword.h"

#if __cplusplus
 extern "C" {
#endif

/************************************************************************/
/*                                                                      */
/*  Process command line options and wildcard arguments                 */
/*                                                                      */
/************************************************************************/

typedef enum {
      Error_Tag,
      Boolean_Tag,
      Int_Tag,
      Short_Tag,
      Long_Tag,
      Byte_Tag,
      Word_Tag,
      DWord_Tag,
      Float_Tag,
      DFloat_Tag,
      String_Tag
      } TAG_TYPE;

struct Option_Tag {
      int         letter;                 /* Option switch              */
      Boolean_T   case_sense;             /* TRUE = case sensitive      */
      TAG_TYPE    type;                   /* Type of option             */
      void       *buf;                    /* Storage location           */
      char       *min;                    /* Minimum value (string)     */
      char       *max;                    /* Maximum value (string)     */
      char       *Default;                /* Default value (string)     */
};

extern struct Option_Tag options[];

extern int        xargc;
extern char      *xargv[];
extern Boolean_T  getopts_range_err;
extern Boolean_T  xargs_on;

int    getopts(int, char *[]);
double getopts_eval(char *str);

#if __cplusplus
 }
#endif

#endif /* GETOPTS_H */
