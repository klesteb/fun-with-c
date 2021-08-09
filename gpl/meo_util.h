
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                                                                           */
/* TITLE:       meo_util.h                                                   */
/*                                                                           */
/* FACILITY:    Global variables and structures for the MEO package.         */
/*                                                                           */
/* ABSTRACT:    These are the declarations of the data structures and        */
/*              routines to implement the MEO utilities.                     */
/*                                                                           */
/* ENVIRONMENT: VMS/AIX/Linux                                                */
/*                                                                           */
/* Version      Author                                           Date        */
/* -------      ---------------------------------------------    ----------- */
/* 1-000        Kevin Esteb                                      23-Apr-2002 */
/*                                                                           */
/*---------------------------------------------------------------------------*/

#ifndef MEO_UTIL_H
#define MEO_UTIL_H 1

#ifdef __cplusplus
    extern  "C" {
#endif

#include  <stdlib.h>
#include "gpl/ansi_setup.h"             /* ANSI or non-ANSI C?          */

/*----------------------------------------------------------------------*/
/*    Structures and Definitions.                                       */
/*----------------------------------------------------------------------*/

/*----------------------------------------------------------------------*/
/*    Miscellaneous declarations.                                       */
/*----------------------------------------------------------------------*/

typedef  enum  MeoBase {
    MeoNone = 0,
    MeoOctal = 8,
    MeoDecimal = 10,
    MeoHexadecimal = 16,
    MeoText = 26
}  MeoBase ;

extern int meo_util_debug;       /* Global debug switch (1/0 = yes/no). */

/*----------------------------------------------------------------------*/
/*    Public Functions                                                  */
/*----------------------------------------------------------------------*/

extern  int  meo_dump P_((FILE *file,
			 const char *indentation,
			 MeoBase base,
			 int numBytesPerLine,
			 void *address,
			 const char *buffer,
			 int numBytesToDump)) ;

#define  meo_dumpd(file, indentation, address, buffer, numBytesToDump) \
    meo_dump(file, indentation, MeoDecimal, 8, address, buffer, numBytesToDump)

#define  meo_dumpo(file, indentation, address, buffer, numBytesToDump) \
    meo_dump(file, indentation, MeoOctal, 8, address, buffer, numBytesToDump)

#define  meo_dumpt(file, indentation, address, buffer, numBytesToDump) \
    meo_dump(file, indentation, MeoText, 40, address, buffer, numBytesToDump)

#define  meo_dumpx(file, indentation, address, buffer, numBytesToDump) \
    meo_dump(file, indentation, MeoHexadecimal, 16, address, buffer, numBytesToDump)

extern  int  meo_load P_((const char *fileName,
			 long offset,
			 void **startAddress,
			 long *numBytes)) ;

extern  int  meo_save P_((void *startAddress,
			 long numBytes,
			 const char *fileName,
			 long offset)) ;


#ifdef __cplusplus
    }
#endif

#endif

