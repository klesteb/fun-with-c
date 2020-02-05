
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                                                                           */
/* TITLE:       fnm_util.h                                                   */
/*                                                                           */
/* FACILITY:    Global variables and structures for the FNM package.         */
/*                                                                           */
/* ABSTRACT:    These are the declarations of the data structures and        */
/*              routines to implement the FNM utilities.                     */
/*                                                                           */
/* ENVIRONMENT: VMS/AIX/Linux                                                */
/*                                                                           */
/* Version      Author                                           Date        */
/* -------      ---------------------------------------------    ----------- */
/* 1-000        Kevin Esteb                                      23-Apr-2002 */
/*                                                                           */
/*---------------------------------------------------------------------------*/

#ifndef FNM_UTIL_H  
#define FNM_UTIL_H 1

#ifdef __cplusplus
    extern  "C" {
#endif

#include "ansi_setup.h"                 /* ANSI or non-ANSI C?          */

/*----------------------------------------------------------------------*/
/*    Structures and Definitions.                                       */
/*----------------------------------------------------------------------*/

typedef  struct  _FileName  *FileName ; /* Filename handle.             */

typedef  enum  FnmPart {
    FnmPath = 0,                        /* The whole pathname.          */
    FnmNode,                            /* Node name, if specified.     */
    FnmDirectory,                       /* Directory only.              */
    FnmFile,                            /* Name, extension, and version.*/
    FnmName,                            /* File name.                   */
    FnmExtension,                       /* File extension.              */
    FnmVersion                          /* Version number.              */
}  FnmPart ;

/*----------------------------------------------------------------------*/
/*    Miscellaneous declarations.                                       */
/*----------------------------------------------------------------------*/

/*----------------------------------------------------------------------*/
/*    Public Functions                                                  */
/*----------------------------------------------------------------------*/

extern  char  *fnm_build P_((int expand, FnmPart part, const char *path, ...)) ;

extern  FileName  fnm_create P_((int expand, const char *path, ...)) ;

extern  int  fnm_destroy P_((FileName filename)) ;

extern  int  fnm_exists P_((const FileName filename)) ;

extern  char  *fnm_parse P_((const FileName fname, FnmPart part)) ;

extern void fnm_vms2unix P_((char *vmspath, char *unixpath, int len));

extern void fnm_unix2vms P_((char *unixpath, char *vmspath, int len));

#define  fnm_path(fname)  fnm_parse(fname, FnmPath)
#define  fnm_node(fname)  fnm_parse(fname, FnmNode)
#define  fnm_directory(fname)  fnm_parse(fname, FnmDirectory)
#define  fnm_file(fname)  fnm_parse(fname, FnmFile)
#define  fnm_name(fname)  fnm_parse(fname, FnmName)
#define  fnm_extension(fname)  fnm_parse(fname, FnmExtension)
#define  fnm_version(fname)  fnm_parse(fname, FnmVersion)

#ifdef __cplusplus
    }
#endif

#endif

