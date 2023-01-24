
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

#ifndef _KEYS_H_
#define _KEYS_H_

/*-------------------------------------------------------------*/
/* constants                                                   */
/*-------------------------------------------------------------*/

/* not sure why ncurses dosen't define this */

#ifndef KEY_TAB
#define KEY_TAB '\t'
#endif

/* useful for checking for control keys */

#ifndef KEY_CTRL
#define KEY_CTRL(c) ((c) & 037)
#endif

#endif

