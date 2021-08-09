
/*---------------------------------------------------------------------------*/
/*                                                                           */
/* TITLE:       cfg_priv.h                                                   */
/*                                                                           */
/* FACILITY:    Private variables and structures for the CFG package.        */
/*                                                                           */
/* ABSTRACT:    These are the declarations of the data structures and        */
/*              routines to implement the CFG utilities.                     */
/*                                                                           */
/* ENVIRONMENT: VMS/Linux                                                    */
/*                                                                           */
/* Version      Author                                           Date        */
/* -------      ---------------------------------------------    ----------- */
/* 1-000        Kevin Esteb                                      23-Apr-2002 */
/*                                                                           */
/*---------------------------------------------------------------------------*/

#ifndef CFG_PRIV_H
#define CFG_PRIV_H 1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "../cfg_util.h"

#define CFGBUF 1024
#define CFG_K_NEWFILE 1
#define CFG_K_OPENED  2
#define CFG_K_UPDATED 3
#define CFG_K_DESTORY 4

extern int cfg_util_debug;          /* Global debug switch (1/0 = yes/no).*/

extern int   section_find(void *data, void *section);
extern void *xmalloc(int);
extern int   stricmp(char *, char *);
extern int   strnicmp(char *, char *, int);
extern char *trim(char *);
extern char *left(const char *, int);
extern char *right(const char *, int);
extern int   pos(const char *, const char *, int);

#endif

