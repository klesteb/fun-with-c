
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

#ifndef _ERROR_TRACE_H_
#define _ERROR_TRACE_H_

/**
 * @file error_trace.h
 * @author Kevin L. Esteb (kevin@kesteb.us)
 * @date 15-Nov-2019
 * 
 * @brief define an error trace structure
 *
 * @par Description
 * This is used to keep track of errors and where they happened. 
 * 
 * @struct error_trace_t error_trace.h "include/error_trace.h"
 * 
 **/
typedef struct _error_trace_s {
    int errnum;                 /**< the error number                   */
    int lineno;                 /**< the line number in the source file */ 
    char *filename;             /**< the source file name               */
    char *function;             /**< the function name                  */
} error_trace_t;

#endif

