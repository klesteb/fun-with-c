
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

#ifndef _ERROR_CODE_H_
#define _ERROR_CODE_H_

/**
 * @file error_code.h
 * @author Kevin L. Esteb (kevin@kesteb.us)
 * @date 15-Nov-2019
 * 
 * @brief A data structure to hold error codes and messages.
 * 
 * @par Description
 * This data structure is used to hold a unified table of error
 * codes and there associated error message. It also loads the
 * operating system error table.
 * 
 * @struct error_code_t error_code.h "include/error_code.h"
 * 
 **/

typedef struct _error_code_s {
    int ec;                    /**< the error code                      */
    char *text;                /**< the text nemonic for the error code */
    char *message;             /**< message text to explain the error   */
} error_code_t;

#endif

