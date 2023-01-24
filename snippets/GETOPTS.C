/* +++Date last modified: 05-Jul-1997 */

/************************************************************************/
/*                                                                      */
/*  GETOPTS.C - Universal command line options parser                   */
/*                                                                      */
/*  Original Copyright 1990-96 by Robert B. Stout as part of            */
/*  the MicroFirm Function Library (MFL)                                */
/*                                                                      */
/*  The user is granted a free limited license to use this source file  */
/*  to create royalty-free programs, subject to the terms of the        */
/*  license restrictions specified in the LICENSE.MFL file.             */
/*                                                                      */
/************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include "sniptype.h"
#include "filnames.h"
#include "errors.h"
#include "dirport.h"
#include "snipmath.h"
#include "minmax.h"
#include "getopts.h"
#include "numcnvrt.h"

#define MAX_XARGS 512

int         xargc = 0;
char       *xargv[MAX_XARGS]        = {NULL};
Boolean_T   getopts_range_err       = False_;
Boolean_T   xargs_on                = True_;

static FILE *rspfile = NULL;
static int   count = 0, argidx = 0;

enum proc_stat { PSerror = -1, PSok, PSliteral};

static Boolean_T      PASCAL bounds(struct Option_Tag *);
static enum proc_stat PASCAL swProc(char *swStr);
static void           PASCAL argProc(char *argStr);

/*
**  getopts()
**
**  Parameters: 1 - argc from main()
**              2 - argv from main()
**              3 - your program's options[] array
**
**  Returns: Number of options specified or Error_
**
**  Notes: 1. Your program should declare the global options[] array which
**            specifies all options recognized by getopts().
**
**         2. Out of range data are coerced into range and getopts_range_err
**            is set True_.
*/

int getopts(int argc, char *argv[])
{
      int i;
      Boolean_T scanning = True_;
      struct Option_Tag *ptr;
      char rspfname[FILENAME_MAX];
      char newarg[256];
      enum proc_stat ps;

      xargc = argc;
      xargv[argidx++] = argv[0];
      for (i = 1, count = 0; i < argc; )
      {
            /*
            ** If necessary, open a response file
            */

            if (scanning && !rspfile && '@' == argv[i][0])
            {
                  rspfile = cant(&argv[i][1], "r");
                  --xargc;
                  continue;
            }

            /*
            ** Get the next argument
            */

            if (rspfile)
            {
                  if (NULL == fgets(newarg, 256, rspfile))
                  {
                        rspfile = NULL;
                        ++i;
                        continue;
                  }
                  else
                  {
                        if ('\n' == LAST_CHAR(newarg))
                              LAST_CHAR(newarg) = NUL;
                  }
            }
            else
            {
                  strcpy(newarg, argv[i++]);
            }

            /*
            ** Per Unix tradition, back-to-back switch characters signify
            ** the end of the switches
            */

            if ((2 == strlen(newarg)) && strchr("-/", newarg[0]) &&
                  strchr("-/", newarg[1]))
            {
                  scanning = False_;
                  if (!rspfile)
                        --xargc;
                  continue;
            }

            if (scanning && (strchr("-/", newarg[0])))
            {
                  ps = swProc(newarg);

                  if (PSerror == ps)
                        return Error_;

                  if (PSok == ps)
                        continue;
            }

            /*
            ** If we got here, newarg must be an argument or filename
            */

            argProc(newarg);
      }
      return count;
}

/*
**  Static function to process switch statements
**
**  Parameters: 1 - argv[i] containing the switch
**
**  Returns: PSok      if switch successful
**           PSerror   if invalid
**           PSliteral if literal (non-switch) argument
*/

static enum proc_stat PASCAL swProc(char *swStr)
{
      struct Option_Tag *ptr;
      Boolean_T searching;
      unsigned short byte_var;
      char *arg_ptr;

      /*
      ** Found a switch - If the 2nd character is also a switch
      ** character. If so, then it's a literal and is skipped
      */

      if (strchr("-/@", swStr[1]))
            return PSliteral;

      for (ptr = options, searching = True_; searching; ++ptr)
      {
            if (!ptr->case_sense)
            {
                  ptr->letter = toupper(ptr->letter);
                  swStr[1]    = toupper(swStr[1]);
            }
            if ((int)swStr[1] == ptr->letter) switch (ptr->type)
            {
            case Boolean_Tag:
                  if ('-' == swStr[2])
                        *((Boolean_T *)(ptr->buf)) = False_;
                  else  *((Boolean_T *)(ptr->buf)) = True_;
                  searching = False_;
                  break;

            case Byte_Tag:
                  if (!swStr[2])
                  {
                        if (ptr->Default)
                              arg_ptr = ptr->Default;
                        else  return PSerror;
                  }
                  else  arg_ptr = &swStr[2];

                  sscanf(arg_ptr, "%hx", &byte_var);
                  *((char *)(ptr->buf)) = (unsigned char)(byte_var & 0xff);
                  bounds(ptr);
                  searching = False_;
                  break;

            case Int_Tag:
                  if (!swStr[2])
                  {
                        if (ptr->Default)
                              arg_ptr = ptr->Default;
                        else  return PSerror;
                  }
                  else  arg_ptr = &swStr[2];

                  *((int *)(ptr->buf)) = (int)(dround(getopts_eval(arg_ptr)));
                  bounds(ptr);
                  searching = False_;
                  break;

            case Short_Tag:
                  if (!swStr[2])
                  {
                        if (ptr->Default)
                              arg_ptr = ptr->Default;
                        else  return PSerror;
                  }
                  else  arg_ptr = &swStr[2];

                  *((short *)(ptr->buf)) =
                        (short)(dround(getopts_eval(arg_ptr)));
                  bounds(ptr);
                  searching = False_;
                  break;

            case Word_Tag:
                  if (!swStr[2])
                  {
                        if (ptr->Default)
                              arg_ptr = ptr->Default;
                        else  return PSerror;
                  }
                  else  arg_ptr = &swStr[2];

                  sscanf(arg_ptr, "%hx", (unsigned short *)(ptr->buf));
                  bounds(ptr);
                  searching = False_;
                  break;

            case Long_Tag:
                  if (!swStr[2])
                  {
                        if (ptr->Default)
                              arg_ptr = ptr->Default;
                        else  return PSerror;
                  }
                  else  arg_ptr = &swStr[2];

                  *((long *)(ptr->buf)) =
                        (long)(dround(getopts_eval(arg_ptr)));
                  bounds(ptr);
                  searching = False_;
                  break;

            case DWord_Tag:
                  if (!swStr[2])
                  {
                        if (ptr->Default)
                              arg_ptr = ptr->Default;
                        else  return PSerror;
                  }
                  else  arg_ptr = &swStr[2];

                  sscanf(arg_ptr, "%lx", (unsigned long *)(ptr->buf));
                  bounds(ptr);
                  searching = False_;
                  break;

            case Float_Tag:
                  if (!swStr[2])
                  {
                        if (ptr->Default)
                              arg_ptr = ptr->Default;
                        else  return PSerror;
                  }
                  else  arg_ptr = &swStr[2];

                  *((double *)(ptr->buf)) = (double)getopts_eval(arg_ptr);
                  bounds(ptr);
                  searching = False_;
                  break;

            case DFloat_Tag:
                  if (!swStr[2])
                  {
                        if (ptr->Default)
                              arg_ptr = ptr->Default;
                        else  return PSerror;
                  }
                  else  arg_ptr = &swStr[2];

                  *((double *)(ptr->buf)) = (double)getopts_eval(arg_ptr);
                  bounds(ptr);
                  searching = False_;
                  break;

            case String_Tag:
                  if (!swStr[2] && ptr->Default)
                        strcpy(ptr->buf, (char *)(ptr->Default));
                  else  strcpy(ptr->buf, &swStr[2]);

                  searching = False_;
                  break;

            default:
                  return Error_;
            }
      }
      ++count;
      if (!rspfile)
            --xargc;

      return PSok;
}

/*
**  Static function to process arguments
*/

static void PASCAL argProc(char *argStr)
{
      DOSFileData ff;

      /* If no wildcards or ignoring wildcards, just copy it */

      if (!xargs_on || !has_wild(argStr))
      {
            xargv[argidx] = malloc(strlen(argStr) + 1);
            if (NULL == xargv[argidx])
                  ErrExit("Out of memory");
            strcpy(xargv[argidx], argStr);
            ++argidx;
            return;
      }
      else  /* Expand wildcards, if possible                      */
      {
            if (Success_ == FIND_FIRST(argStr, _A_ANY, &ff))
            {
                  char path[FILENAME_MAX];
                  char *p;

                  /* Save the path for re-attachment              */

                  fnSplit(argStr, NULL, path, NULL, NULL, NULL, NULL);

                  --xargc;    /* We add stuff in the loop, so back up   */
                  do
                  {                             
                        xargv[argidx] = malloc(strlen(ff_name(&ff))
                                               + strlen(path) + 2);
                        if (NULL == xargv[argidx])
                              ErrExit("Out of memory");
                        fnMerge(xargv[argidx], NULL, path, NULL, ff_name(&ff),
                                NULL, NULL);
                        ++argidx;
                        ++xargc;

                  } while (Success_ == FIND_NEXT(&ff));
                  FIND_END(&ff);
            }
      }
}

/*
**  Assure new data are within specified ranges, return non-zero if coerced
*/

static Boolean_T PASCAL bounds(struct Option_Tag *option)
{
      Boolean_T coerced = False_;
      union {
            unsigned char     B;
            int               I;
            short             S;
            unsigned short    W;
            long              L;
            unsigned long     DW;
            float             F;
            double            D;
      } tmp, val;
      
      switch(option->type)
      {
      case Byte_Tag:
            tmp.B = *((unsigned char *)(option->buf));
            if (option->max)
            {
                  sscanf(option->max, "%hx", &val.B);
                  tmp.B = min(tmp.B, val.B);
            }
            if (option->min)
            {
                  sscanf(option->min, "%hx", &val.B);
                  tmp.B = max(tmp.B, val.B);
            }
            if (*((unsigned char *)(option->buf)) != tmp.B)
            {
                  getopts_range_err = True_;
                  *((unsigned char *)(option->buf)) = tmp.B;
                  coerced = True_;
            }
            break;

      case Int_Tag:
            tmp.I = *((int *)(option->buf));
            if (option->max)
            {
                  val.D = dround(getopts_eval(option->max));
                  if (val.D > (double)INT_MAX)
                        val.I = INT_MAX;
                  else  val.I = (int)val.D;
                  tmp.I = min(tmp.I, val.I);
            }
            if (option->min)
            {
                  val.D = dround(getopts_eval(option->min));
                  if (val.D < (double)INT_MIN)
                        val.I = INT_MIN;
                  else  val.I = (int)val.D;
                  tmp.I = max(tmp.I, val.I);
            }
            if (*((int *)(option->buf)) != tmp.I)
            {
                  getopts_range_err = True_;
                  *((int *)(option->buf)) = tmp.I;
                  coerced = True_;
            }
            break;

      case Short_Tag:
            tmp.S = *((short *)(option->buf));
            if (option->max)
            {
                  val.D = dround(getopts_eval(option->max));
                  if (val.D > (double)SHRT_MAX)
                        val.S = SHRT_MAX;
                  else  val.S = (short)val.D;
                  tmp.S = min(tmp.I, val.I);
            }
            if (option->min)
            {
                  val.D = dround(getopts_eval(option->min));
                  if (val.D < (double)SHRT_MIN)
                        val.S = SHRT_MIN;
                  else  val.S = (short)val.D;
                  tmp.S = max(tmp.I, val.I);
            }
            if (*((short *)(option->buf)) != tmp.S)
            {
                  getopts_range_err = True_;
                  *((short *)(option->buf)) = tmp.I;
                  coerced = True_;
            }
            break;

      case Word_Tag:
            tmp.W = *((unsigned short *)(option->buf));
            if (option->max)
            {
                  sscanf(option->max, "%hx", &val.W);
                  tmp.W = min(tmp.W, val.W);
            }
            if (option->min)
            {
                  sscanf(option->min, "%hx", &val.W);
                  tmp.W = max(tmp.W, val.W);
            }
            if (*((unsigned short *)(option->buf)) != tmp.W)
            {
                  getopts_range_err = True_;
                  *((unsigned short *)(option->buf)) = tmp.W;
                  coerced = True_;
            }
            break;

      case Long_Tag:
            tmp.L = *((long *)(option->buf));
            if (option->max)
            {
                  val.D = dround(getopts_eval(option->max));
                  if (val.D > (double)LONG_MAX)
                        val.L = LONG_MAX;
                  else  val.L = (long)val.D;
                  tmp.L = min(tmp.L, val.L);
            }
            if (option->min)
            {
                  val.D = dround(getopts_eval(option->min));
                  if (val.D < (double)LONG_MIN)
                        val.L = LONG_MIN;
                  else  val.L = (int)val.D;
                  tmp.L = max(tmp.L, val.L);
            }
            if (*((long *)(option->buf)) != tmp.L)
            {
                  getopts_range_err = True_;
                  *((long *)(option->buf)) = tmp.L;
                  coerced = True_;
            }
            break;

      case DWord_Tag:
            tmp.DW = *((unsigned long *)(option->buf));
            if (option->max)
            {
                  sscanf(option->max, "%lx", &val.DW);
                  tmp.DW = min(tmp.DW, val.DW);
            }
            if (option->min)
            {
                  sscanf(option->min, "%hx", &val.DW);
                  tmp.DW = max(tmp.DW, val.DW);
            }
            if (*((unsigned long *)(option->buf)) != tmp.DW)
            {
                  getopts_range_err = True_;
                  *((unsigned long *)(option->buf)) = tmp.DW;
                  coerced = True_;
            }
            break;

      case Float_Tag:
            tmp.F = *((float *)(option->buf));
            if (option->max)
            {
                  val.F = (float)getopts_eval(option->max);
                  tmp.F = min(tmp.F, val.F);
            }
            if (option->min)
            {
                  val.F = (float)getopts_eval(option->min);
                  tmp.F = max(tmp.F, val.F);
            }
            if (*((float *)(option->buf)) != tmp.F)
            {
                  getopts_range_err = True_;
                  *((float *)(option->buf)) = tmp.F;
                  coerced = True_;
            }
            break;

      case DFloat_Tag:
            tmp.D = *((double *)(option->buf));
            if (option->max)
            {
                  val.D = getopts_eval(option->max);
                  tmp.D = min(tmp.D, val.D);
            }
            if (option->min)
            {
                  val.D = getopts_eval(option->min);
                  tmp.D = max(tmp.D, val.D);
            }
            if (*((double *)(option->buf)) != tmp.D)
            {
                  getopts_range_err = True_;
                  *((double *)(option->buf)) = tmp.D;
                  coerced = True_;
            }
            break;
      }

      return coerced;
}

/*
**  Simplified evaluate() call - returns double or aborts
*/

double getopts_eval(char *str)
{
      double retval;

      if (Success_ == evaluate(str, &retval))
            return retval;
      else  ErrExit("Error evlauating \"%s\" - aborting\n", str);
}
