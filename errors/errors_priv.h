
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

#ifndef _ERRORS_PRIV_H_
#define _ERRORS_PRIV_H_

#include "error_code.h"
#include "error_codes.h"

/*-------------------------------------------------------------*/
/* error code definations                                      */
/*-------------------------------------------------------------*/

error_code_t error_codes[] = {
    { E_OK,      "E_OK",      "Everything is AOK" },
    { E_INVPARM, "E_INVPARM", "Invalid parameters" },
    { E_NODATA,  "E_NODATA",  "No data available" },
    { E_INVOPS,  "E_INVOPS",  "An invalid operation was tried" },
    { E_INVOBJ,  "E_INVOBJ",  "Invalid object" },
    { E_NOLOAD,  "E_NOLOAD",  "Unable to load data" },
    { E_NOTSAME, "E_NOTSAME", "The objects are not the same" },
    { E_NOCREAT, "E_NOCREAT", "The object was not created" },
    { E_NOCOLOR, "E_NOCOLOR", "The terminal doesn't support color" }
};

#endif

