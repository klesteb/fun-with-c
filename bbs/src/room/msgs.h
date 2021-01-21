
/*---------------------------------------------------------------------------*/
/*                Copyright (c) 2021 by Kevin L. Esteb                       */
/*                                                                           */
/*  Permission to use, copy, modify, and distribute this software and its    */
/*  documentation for any purpose and without fee is hereby granted,         */
/*  provided that this copyright notice appears in all copies. The author    */
/*  makes no representations about the suitability of this software for      */
/*  any purpose. It is provided "as is" without express or implied warranty. */
/*                                                                           */
/*---------------------------------------------------------------------------*/

#ifndef _MSGS_H
#define _MSGS_H

#include "handler.h"

/*-------------------------------------------------------------*/
/* constants                                                   */
/*-------------------------------------------------------------*/

#define MAILROOM 1          /* mail is always room 0           */
#define LOBBY    2          /* lobby is always room 1          */
#define AIDEROOM 3          /* aide is always room 2           */

/*----------------------------------------------------------------*/
/* interface                                                      */
/*----------------------------------------------------------------*/

extern handler_t *msgs_create(files_t *, char *, int, int, int, tracer_t *);

#endif

