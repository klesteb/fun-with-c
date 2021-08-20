
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
#include <errno.h>

#include "gpl/fnm_util.h"
#include "include/when.h"
#include "objects/object.h"
#include "include/error_codes.h"

#include "bbs/src/door/door.h"
#include "bbs/src/room/room.h"
#include "bbs/src/room/doors.h"
#include "bbs/src/room/handler.h"

require_klass(HANDLER_KLASS);

/*----------------------------------------------------------------*/
/* klass overrides                                                */
/*----------------------------------------------------------------*/

int _doors_attach(handler_t *self, room_base_t *room) {

    char name[7];
    int stat = OK;
    door_t *doors = NULL;

    when_error_in {

        memset(name, '\0', 7);
        snprintf(name, 6, "%05d", (int)room->conference);

fprintf(stderr, "_doors_attach() - path: %s\n", room->path);
fprintf(stderr, "_doors_attach() - name: %s\n", name);

        doors = door_create(room->path, name, room->retries, room->timeout, self->trace);
        check_creation(doors);
fprintf(stderr, "_doors_attach() - after doors_create()\n");

        stat = door_open(doors);
        check_return(stat, doors);
fprintf(stderr, "_doors_attach() - after doors_open()\n");

        (*self).handle = (void *)doors;

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int _doors_detach(handler_t *self) {

    int stat = OK;
    door_t *doors = (door_t *)self->handle;

fprintf(stderr, "_doors_detach()\n");

    when_error_in {

        if (doors != NULL) {

            stat = door_close(doors);
            check_return(stat, doors);

            stat = door_destroy(doors);
            check_return(stat, doors);

        }

        (*self).handle = NULL;

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int _doors_remove(handler_t *self) {

    int stat = OK;
    door_t *doors = (door_t *)self->handle;

fprintf(stderr, "_doors_remove()\n");

    when_error_in {

        if (doors != NULL) {

            stat = door_remove(doors);
            check_return(stat, doors);

            stat = door_destroy(doors);
            check_return(stat, doors);

        }

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int _doors_handle(handler_t *self, void **handle) {

    int stat = OK;
    door_t *doors = (door_t *)self->handle;

fprintf(stderr, "_doors_handle()\n");

    when_error_in {

        errno = 0;
        *handle = calloc(1, sizeof(door_t));
        if (*handle == NULL) {

            cause_error(errno);

        }

        memcpy(*handle, doors, sizeof(door_t));

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

/*----------------------------------------------------------------*/
/* klass implementation                                           */
/*----------------------------------------------------------------*/

handler_t *doors_create(files_t *db, char *path, int retries, int timeout, int base, tracer_t *dump) {

    int stat = ERR;
    item_list_t items[5];
    handler_t *temp = NULL;

    if ((temp = handler_create(db, path, retries, timeout, base, dump))) {

        SET_ITEM(items[0], HANDLER_M_ATTACH, _doors_attach, 0, NULL);
        SET_ITEM(items[1], HANDLER_M_DETACH, _doors_detach, 0, NULL);
        SET_ITEM(items[2], HANDLER_M_REMOVE, _doors_remove, 0, NULL);
        SET_ITEM(items[3], HANDLER_M_HANDLE, _doors_handle, 0, NULL);
        SET_ITEM(items[4], 0, 0, 0, 0);

        stat = handler_override(temp, items);
        if (stat != OK) {

            handler_destroy(temp);
            temp = NULL;

        }

    }

    return temp;

}

