
/*---------------------------------------------------------------------------*/
/*                Copyright (c) 2021 by Kevin L. Esteb                       */
/*                                                                           */
/*  Permission to use, copy, modify, and distribute this software and its    */
/*  documentation for any purpose and without fee is hereby granted,         */
/*  provided that this copyright notice appears in all copies. The author    */
/*  makes no representations about the suitability of this software for      */
/*  any purpose. It is provided "as is" without express or implied warranty. */
/*                                                                           */
/*---------------------------------------------------------------------------*/

#include <stdio.h>
#include "finds.h"

int find_users_all(void *data, int len, user_base_t *user) {

    int stat = FALSE;

    if (!(user->flags & US_DELETED) &&
        !(user->flags & US_INACTIVE)) {

        stat = TRUE;

    }

    return stat;

}

int find_user_by_name(void *data, int len, user_base_t *user) {

    int stat = FALSE;
    char username[LEN_NAME+1];

    memset(username, '\0', LEN_NAME+1);
    strncpy(username, (char *)data, LEN_NAME);

    if (!(user->flags & US_INACTIVE)) {

        if (strcmp(user->username, username) == 0) {

            stat = TRUE;

        }

    }

    return stat;

}

