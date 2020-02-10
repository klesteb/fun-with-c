
/*---------------------------------------------------------------------------*/
/*                Copyright (c) 2019 by Kevin L. Esteb                       */
/*                                                                           */
/*  Permission to use, copy, modify, and distribute this software and its    */
/*  documentation for any purpose and without fee is hereby granted,         */
/*  provided that the above copyright notice appears in all copies. The      */
/*  author makes no representations about the suitability of this software   */
/*  for any purpose. It is provided "as is" without express or implied       */
/*  warranty.                                                                */
/*---------------------------------------------------------------------------*/

#ifndef _COMMON_H_
#define _COMMON_H_

#include "errors.h"
#include "workbench.h"
#include "error_codes.h"

/*----------------------------------------------------------------*/
/* singletons                                                     */
/*----------------------------------------------------------------*/

extern errors_t    *errs;
extern workbench_t *wb;

#endif

