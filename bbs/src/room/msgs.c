
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

#include "bbs/src/jam/jam.h"
#include "bbs/src/room/msgs.h"
#include "bbs/src/room/room.h"
#include "bbs/src/room/handler.h"

require_klass(HANDLER_KLASS);

/*----------------------------------------------------------------*/
/* klass overrides                                                */
/*----------------------------------------------------------------*/

int _msgs_init(handler_t *self) {

    int x = 0;
    int stat = OK;
    int revision = 1;
    room_base_t aide;
    room_base_t email;
    room_base_t lobby;
    ssize_t count = 0;
    ssize_t position = 0;
    int recsize = sizeof(room_base_t);

    when_error_in {

        memset(&aide, '\0', recsize);
        memset(&email, '\0', recsize);
        memset(&lobby, '\0', recsize);

        stat = files_seek(self->db, 0, SEEK_SET);
        check_return(stat, self->db);

        lobby.aide = 1;
        lobby.roomnum = LOBBY;
        lobby.base = self->base;
        lobby.conference = LOBBY;
        lobby.revision = revision;
        strcpy(lobby.name, "Lobby");
        lobby.retries = self->retries;
        lobby.timeout = self->timeout;
        strcpy(lobby.description, "The general message area");
        lobby.flags = (RM_PERMROOM | RM_PUBLIC | RM_INUSE | RM_MESSAGES);
        strncpy(lobby.path, fnm_build(1, FnmPath, self->path, NULL), 255);

        for (x = 0; x < USERNUM; x++) {

            lobby.status[x] = 0;

        }

        stat = files_tell(self->db, &position);
        check_return(stat, self->db);

        stat = files_lock(self->db, position, recsize);
        check_return(stat, self);

        stat = files_write(self->db, &lobby, recsize, &count);
        check_return(stat, self);

        stat = files_unlock(self->db);
        check_return(stat, self);

        email.aide = 1;
        email.roomnum = MAILROOM;
        email.base = self->base;
        email.revision = revision;
        strcpy(email.name, "Mail");
        email.conference = MAILROOM;
        email.retries = self->retries;
        email.timeout = self->timeout;
        strcpy(email.description, "Personal messages");
        email.flags = (RM_PERMROOM | RM_PRIVATE | RM_INUSE | RM_MESSAGES);
        strncpy(email.path, fnm_build(1, FnmPath, self->path, NULL), 255);

        for (x = 0; x < USERNUM; x++) {

            email.status[x] = 0;

        }
        
        stat = files_tell(self->db, &position);
        check_return(stat, self->db);

        stat = files_lock(self->db, position, recsize);
        check_return(stat, self);

        stat = files_write(self->db, &email, recsize, &count);
        check_return(stat, self);

        stat = files_unlock(self->db);
        check_return(stat, self);

        aide.aide = 1;
        aide.roomnum = AIDEROOM;
        aide.base = self->base;
        aide.revision = revision;
        strcpy(aide.name, "Aide");
        aide.conference = AIDEROOM;
        aide.retries = self->retries;
        aide.timeout = self->timeout;
        strcpy(aide.description, "Private aide messages");
        aide.flags = (RM_PERMROOM | RM_PRIVATE | RM_INUSE | RM_MESSAGES);
        strncpy(aide.path, fnm_build(1, FnmPath, self->path, NULL), 255);

        for (x = 0; x < USERNUM; x++) {

            aide.status[x] = 0;

        }

        stat = files_tell(self->db, &position);
        check_return(stat, self->db);

        stat = files_lock(self->db, position, recsize);
        check_return(stat, self);

        stat = files_write(self->db, &aide, recsize, &count);
        check_return(stat, self);

        stat = files_unlock(self->db);
        check_return(stat, self);

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int _msgs_attach(handler_t *self, room_base_t *room) {

    char name[7];
    int stat = OK;
    jam_t *jam = NULL;

    when_error_in {

        memset(name, '\0', 7);
        snprintf(name, 6, "%05d", (int)room->conference);

        jam = jam_create(room->path, name, room->retries, room->timeout, room->base, self->trace);
        check_creation(jam);

        stat = jam_open(jam);
        check_return(stat, jam);

        (*self).handle = (void *)jam;

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int _msgs_detach(handler_t *self) {

    int stat = OK;
    jam_t *jam = (jam_t *)self->handle;

    when_error_in {

        stat = jam_close(jam);
        check_return(stat, jam);

        stat = jam_destroy(jam);
        check_return(stat, jam);

        (*self).handle = NULL;

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int _msgs_remove(handler_t *self) {

    int stat = OK;
    jam_t *jam = (jam_t *)self->handle;

    when_error_in {

        stat = jam_remove(jam);
        check_return(stat, jam);

        stat = jam_destroy(jam);
        check_return(stat, jam);

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int _msgs_handle(handler_t *self, void **handle) {

    int stat = OK;
    jam_t *jam = (jam_t *)self->handle;

    when_error_in {

        errno = 0;
        *handle = calloc(1, sizeof(jam_t));
        if (*handle == NULL) {

            cause_error(errno);

        }

        memcpy(*handle, jam, sizeof(jam_t));

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

handler_t *msgs_create(files_t *db, char *path, int retries, int timeout, int base, tracer_t *dump) {

    int stat = ERR;
    item_list_t items[6];
    handler_t *temp = NULL;

    if ((temp = handler_create(db, path, retries, timeout, base, dump))) {

        SET_ITEM(items[0], HANDLER_M_INIT, _msgs_init, 0, NULL);
        SET_ITEM(items[1], HANDLER_M_ATTACH, _msgs_attach, 0, NULL);
        SET_ITEM(items[2], HANDLER_M_DETACH, _msgs_detach, 0, NULL);
        SET_ITEM(items[3], HANDLER_M_REMOVE, _msgs_remove, 0, NULL);
        SET_ITEM(items[4], HANDLER_M_HANDLE, _msgs_handle, 0, NULL);
        SET_ITEM(items[5], 0, 0, 0, 0);

        stat = handler_override(temp, items);
        if (stat != OK) {

            handler_destroy(temp);
            temp = NULL;

        }

    }

    return temp;

}

