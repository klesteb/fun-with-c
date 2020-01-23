
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

#ifndef _MENU_PRIV_H
#define _MENU_PRIV_H

/*----------------------------------------------------------------*/
/* private data                                                   */
/*----------------------------------------------------------------*/

typedef struct _menu_data_s {
    MENU *menu;
    ITEM **items;
    Menu_Options options;
    char *mark;
} menu_data_t;

/*----------------------------------------------------------------*/
/* common methods                                                 */
/*----------------------------------------------------------------*/

extern int _menu_dtor(object_t *);
extern int _menu_draw(container_t *);
extern int _menu_erase(container_t *);
extern int _menu_remove(container_t *);
extern int _menu_display(container_t *);
extern int _menu_refresh(container_t *);
extern int _menu_remove_component(container_t *, component_t *);

#endif

