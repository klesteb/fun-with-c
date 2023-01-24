
/*---------------------------------------------------------------------------*/
/*                Copyright (c) 2020 by Kevin L. Esteb                       */
/*                                                                           */
/*  Permission to use, copy, modify, and distribute this software and its    */
/*  documentation for any purpose and without fee is hereby granted,         */
/*  provided that the above copyright notice appears in all copies. The      */
/*  author makes no representations about the suitability of this software   */
/*  for any purpose. It is provided "as is" without express or implied       */
/*  warranty.                                                                */
/*---------------------------------------------------------------------------*/

#ifndef _TEMPLATE_KLASS_H
#define _TEMPLATE_KLASS_H

/*-------------------------------------------------------------*/
/* klass public interface                                      */
/*-------------------------------------------------------------*/

typedef struct _template_s template_t;

/*-------------------------------------------------------------*/
/* constants                                                   */
/*-------------------------------------------------------------*/

#define TEMPLATE(x) ((template_t *)(x))

#define TEMPLATE_M_DESTRUCTOR 1

#endif

