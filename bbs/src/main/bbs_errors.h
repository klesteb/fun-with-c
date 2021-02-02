
/*---------------------------------------------------------------------------*/
/*                Copyright (c) 2021 by Kevin L. Esteb                       */
/*                                                                           */
/*  Permission to use, copy, modify, and distribute this software and its    */
/*  documentation for any purpose and without fee is hereby granted,         */
/*  provided that the above copyright notice appears in all copies. The      */
/*  author makes no representations about the suitability of this software   */
/*  for any purpose. It is provided "as is" without express or implied       */
/*  warranty.                                                                */
/*---------------------------------------------------------------------------*/

#ifndef _ERRORS_BBS_H_
#define _ERRORS_BBS_H_

#include "error_code.h"
#include "bbs_error_codes.h"

/*-------------------------------------------------------------*/
/* bbs error code definations                                  */
/*-------------------------------------------------------------*/

error_code_t bbs_codes[] = {
    { E_UNKUSER, "E_UNKUSER", "Unknown bbs user name" },
    { E_UNKROOM, "E_UNKROOM", "Unknown bbs room" },
    { E_UNKNODE, "E_UNKNODE", "Unknown bbs node" },
};

#endif

