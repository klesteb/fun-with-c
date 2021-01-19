
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

#include "msgs.h"
#include "room.h"
#include "when.h"
#include "files.h"
#include "object.h"
#include "jam/jam.h"
#include "fnm_util.h"
#include "error_codes.h"

require_klass(ROOM_KLASS);

/*----------------------------------------------------------------*/
/* klass overrides                                                */
/*----------------------------------------------------------------*/

int _msgs_init(room_t *self) {

    int stat = OK;
    int revision = 1;
    room_base_t aide;
    room_base_t email;
    room_base_t lobby;
    ssize_t count = 0;
    ssize_t position = 0;

    when_error_in {

        memset(&aide, '\0', sizeof(room_base_t));
        memset(&email, '\0', sizeof(room_base_t));
        memset(&lobby, '\0', sizeof(room_base_t));

        stat = files_seek(self->roomdb, 0, SEEK_SET);
        check_return(stat, self->roomdb);

        email.aide = 1;
        email.roomnum = 1;
        email.base = self->base;
        email.revision = revision;
        strcpy(email.name, "Mail");
        email.conference = MAILROOM;
        email.retries = self->retries;
        email.timeout = self->timeout;
        email.flags = (RM_PERMROOM | RM_PUBLIC | RM_INUSE | RM_MESSAGES);
        strncpy(email.path, fnm_build(1, FnmPath, self->msgbase, NULL), 255);

        stat = files_tell(self->roomdb, &position);
        check_return(stat, self->roomdb);

        stat = self->_lock(self, position);
        check_return(stat, self);

        stat = self->_write(self, &email, &count);
        check_return(stat, self);

        stat = self->_unlock(self);
        check_return(stat, self);

        lobby.aide = 1;
        lobby.roomnum = 2;
        lobby.base = self->base;
        lobby.conference = LOBBY;
        lobby.revision = revision;
        strcpy(lobby.name, "Lobby");
        lobby.retries = self->retries;
        lobby.timeout = self->timeout;
        lobby.flags = (RM_PERMROOM | RM_PUBLIC | RM_INUSE | RM_MESSAGES);
        strncpy(lobby.path, fnm_build(1, FnmPath, self->msgbase, NULL), 255);

        stat = files_tell(self->roomdb, &position);
        check_return(stat, self->roomdb);

        stat = self->_lock(self, position);
        check_return(stat, self);

        stat = self->_write(self, &lobby, &count);
        check_return(stat, self);

        stat = self->_unlock(self);
        check_return(stat, self);

        aide.aide = 1;
        aide.roomnum = 3;
        aide.base = self->base;
        aide.revision = revision;
        strcpy(aide.name, "Aide");
        aide.conference = AIDEROOM;
        aide.retries = self->retries;
        aide.timeout = self->timeout;
        aide.flags = (RM_PERMROOM | RM_INUSE | RM_MESSAGES);
        strncpy(aide.path, fnm_build(1, FnmPath, self->msgbase, NULL), 255);

        stat = files_tell(self->roomdb, &position);
        check_return(stat, self->roomdb);

        stat = self->_lock(self, position);
        check_return(stat, self);

        stat = self->_write(self, &aide, &count);
        check_return(stat, self);

        stat = self->_unlock(self);
        check_return(stat, self);

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int _msgs_attach(room_t *self, room_base_t *room) {

    char name[7];
    int stat = OK;
    jam_t *jam = NULL;

    when_error_in {

        memset(name, '\0', 7);
        snprintf(name, 6, "%05d", (int)room->conference);

fprintf(stderr, "path = %s\n", room->path);
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

int _msgs_detach(room_t *self) {

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

int _msgs_remove(room_t *self) {

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

/*----------------------------------------------------------------*/
/* klass implementation                                           */
/*----------------------------------------------------------------*/

room_t *msgs_create(char *dbpath, char *msgpath, int rooms, int retries, int timeout, int base, tracer_t *dump) {

    int stat = ERR;
    room_t *temp = NULL;
    item_list_t items[5];

    if ((temp = room_create(dbpath, msgpath, rooms, retries, timeout, base, dump))) {

        SET_ITEM(items[0], ROOM_M_INIT, _msgs_init, 0, NULL);
        SET_ITEM(items[1], ROOM_M_ATTACH, _msgs_attach, 0, NULL);
        SET_ITEM(items[2], ROOM_M_DETACH, _msgs_detach, 0, NULL);
        SET_ITEM(items[3], ROOM_M_REMOVE, _msgs_remove, 0, NULL);
        SET_ITEM(items[4], 0, 0, 0, 0);

        stat = room_override(temp, items);
        if (stat != OK) {
            
            room_destroy(temp);
            temp = NULL;
            
        }

    }

    return temp;

}

