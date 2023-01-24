
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

#ifndef _STATUS_H_
#define _STATUS_H_

/**
 * @file status.h
 * @author Kevin L. Esteb (kevin@kesteb.us)
 * @date 15-Nov-2019
 * 
 * @brief A set of macros to define the return status.
 * 
 * @par Description
 * A set of defines that define the return status of functions. 
 * This leads to a unified system for checking the error returns. 
 * The values are based on ncurses.
 * 
 * @def ERR indicate an error condition
 * @def OK  indicate a success condition
 * 
 **/

#undef  ERR
#define ERR (-1)
#undef  OK
#define OK  (0)

#endif

