
/*---------------------------------------------------------------------------*/
/*  Copyright (c) 1996 by Charles A. Measday                                 */
/*                                                                           */
/*  Permission to use, copy, modify, and distribute this software and its    */
/*  documentation for any purpose and without fee is hereby granted,         */
/*  provided that the above copyright notice appears in all copies. The      */
/*  author makes no representations about the suitability of this software   */
/*  for any purpose. It is provided "as is" without express or implied       */
/*  warranty.                                                                */
/*---------------------------------------------------------------------------*/

#include "opt_priv.h"

/*----------------------------------------------------------------------*/

/**/

int opt_util_debug = 0;          /* Global debug switch (1/0 = yes/no). */

OptContext defaultContext = NULL;    /* Default scan context if the     */
					                 /* caller doesn't specify one.     */
					                 /* (Under VxWorks, this pointer    */
					                 /* is automatically registered     */
					                 /* with the operating system as    */
					                 /* a task variable.)               */

/*----------------------------------------------------------------------*/

