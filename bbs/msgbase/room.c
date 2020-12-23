
/*---------------------------------------------------------------------------*/
/*                Copyright (c) 2020 by Kevin L. Esteb                       */
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

#include "room.h"
#include "when.h"
#include "files.h"
#include "object.h"
#include "fnm_util.h"
#include "error_codes.h"

require_klass(OBJECT_KLASS);

/*----------------------------------------------------------------*/
/* klass methods                                                  */
/*----------------------------------------------------------------*/

int _room_ctor(object_t *, item_list_t *);
int _room_dtor(object_t *);

int _room_compare(room_t *, room_t *);
int _room_override(room_t *, item_list_t *);

int _room_open(room_t *);
int _room_close(room_t *);
int _room_add(room_t *, room_base_t *);
int _room_get(room_t *, short, room_base_t *);
int _room_put(room_t *, short, room_base_t *);
int _room_read(room_t *, room_base_t *, ssize_t *);
int _room_next(room_t *, room_base_t *, ssize_t *);
int _room_prev(room_t *, room_base_t *, ssize_t *);
int _room_last(room_t *, room_base_t *, ssize_t *);
int _room_first(room_t *, room_base_t *, ssize_t *);
int _room_write(room_t *, room_base_t *, ssize_t *);
int _room_build(room_t *, room_base_t *, room_base_t *);

/*----------------------------------------------------------------*/
/* klass declaration                                              */
/*----------------------------------------------------------------*/

declare_klass(ROOM_KLASS) {
    .size = KLASS_SIZE(room_t),
    .name = KLASS_NAME(room_t),
    .ctor = _room_ctor,
    .dtor = _room_dtor,
};

/*----------------------------------------------------------------*/
/* klass interface                                                */
/*----------------------------------------------------------------*/

room_t *room_create(char *dbpath, char *msgpath, int retries, int timeout, tracer_t *dump) {

    int stat = ERR;
    room_t *self = NULL;
    item_list_t items[6];

    SET_ITEM(items[0], ROOM_K_MSGBASE, msgpath, strlen(msgpath), NULL);
    SET_ITEM(items[1], ROOM_K_DATABASE, dbpath, strlen(dbpath), NULL);
    SET_ITEM(items[2], ROOM_K_RETRIES, &retries, sizeof(int), NULL);
    SET_ITEM(items[3], ROOM_K_TIMEOUT, &timeout, sizeof(int), NULL);
    SET_ITEM(items[4], ROOM_K_TRACE, dump, 0, NULL);
    SET_ITEM(items[5], 0,0,0,0);

    self = (room_t *)object_create(ROOM_KLASS, items, &stat);

    return self;

}

int room_destroy(room_t *self) {

    int stat = OK;

    when_error {

        if (self != NULL) {

            if (object_assert(self, room_t)) {

                stat = self->dtor(OBJECT(self));
                check_status(stat, OK, E_INVOPS);

            } else {

                cause_error(E_INVOBJ);

            }

        } else {

            cause_error(E_INVPARM);

        }

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int room_override(room_t *self, item_list_t *items) {

    int stat = OK;

    when_error {
        
        if (self != NULL) {

            stat = self->_override(self, items);
            check_status(stat, OK, E_INVOPS);

        } else {

            cause_error(E_INVPARM);

        }

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int room_compare(room_t *us, room_t *them) {

    int stat = OK;

    when_error {

        if (us != NULL) {

            if (object_assert(them, room_t)) {

                stat = us->_compare(us, them);
                check_status(stat, OK, E_NOTSAME);

            } else {

                cause_error(E_INVOBJ);

            }

        } else {

            cause_error(E_INVPARM);

        }

        exit_when;

    } use {

        stat = ERR;
        process_error(us);

    } end_when;

    return stat;

}

char *room_version(room_t *self) {

    char *version = VERSION;

    return version;

}

int room_open(room_t *self) {

    int stat = OK;

    when_error_in {

        if (self == NULL) {

            cause_error(E_INVPARM);

        }

        stat = self->_open(self);
        check_return(stat, self);

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int room_close(room_t *self) {

    int stat = OK;

    when_error_in {

        if (self == NULL) {

            cause_error(E_INVPARM);

        }

        stat = self->_close(self);
        check_return(stat, self);

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int room_first(room_t *self, room_base_t *room, ssize_t *count) {

    int stat = OK;

    when_error_in {

        if ((self == NULL) || (room == NULL)) {

            cause_error(E_INVPARM);

        }

        stat = self->_first(self, room, count);
        check_return(stat, self);

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int room_next(room_t *self, room_base_t *room, ssize_t *count) {

    int stat = OK;

    when_error_in {

        if ((self == NULL) || (room == NULL)) {

            cause_error(E_INVPARM);

        }

        stat = self->_next(self, room, count);
        check_return(stat, self);

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int room_prev(room_t *self, room_base_t *room, ssize_t *count) {

    int stat = OK;

    when_error_in {

        if ((self == NULL) || (room == NULL)) {

            cause_error(E_INVPARM);

        }

        stat = self->_prev(self, room, count);
        check_return(stat, self);

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int room_last(room_t *self, room_base_t *room, ssize_t *count) {

    int stat = OK;

    when_error_in {

        if ((self == NULL) || (room == NULL)) {

            cause_error(E_INVPARM);

        }

        stat = self->_last(self, room, count);
        check_return(stat, self);

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int room_add(room_t *self, room_base_t *room) {

    int stat = OK;

    when_error_in {

        if ((self == NULL) || (room == NULL)) {

            cause_error(E_INVPARM);

        }

        stat = self->_add(self, room);
        check_return(stat, self);

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int room_get(room_t *self, short conference, room_base_t *room) {

    int stat = OK;

    when_error_in {

        if ((self == NULL) || (room == NULL)) {

            cause_error(E_INVPARM);

        }

        stat = self->_get(self, conference, room);
        check_return(stat, self);

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int room_put(room_t *self, short conference, room_base_t *room) {

    int stat = OK;

    when_error_in {

        if ((self == NULL) || (room == NULL)) {

            cause_error(E_INVPARM);

        }

        stat = self->_put(self, conference, room);
        check_return(stat, self);

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

int _room_ctor(object_t *object, item_list_t *items) {

    int base = 0;
    int stat = ERR;
    int timeout = 1;
    int retries = 30;
    char msgbase[256];
    char database[256];
    char roombase[256];
    room_t *self = NULL;
    tracer_t *dump = NULL;

    if (object != NULL) {

        memset(msgbase, '\0', 256);
        memset(database, '\0', 256);
        memset(roombase, '\0', 256);

        strcpy(msgbase, ".");
        strcpy(database, ".");

        /* capture our items */

        if (items != NULL) {

            int x;
            for (x = 0;; x++) {

                if ((items[x].buffer_length == 0) &&
                    (items[x].item_code == 0)) break;

                switch(items[x].item_code) {
                    case ROOM_K_MSGBASE: {
                        memcpy(&msgbase, 
                               items[x].buffer_address, 
                               items[x].buffer_length);
                        break;
                    }
                    case ROOM_K_DATABASE: {
                        memcpy(&database, 
                               items[x].buffer_address, 
                               items[x].buffer_length);
                        break;
                    }
                    case ROOM_K_RETRIES: {
                        memcpy(&retries, 
                               items[x].buffer_address, 
                               items[x].buffer_length);
                        break;
                    }
                    case ROOM_K_TIMEOUT: {
                        memcpy(&timeout, 
                               items[x].buffer_address, 
                               items[x].buffer_length);
                        break;
                    }
                    case ROOM_K_TRACE: {
                        dump = items[x].buffer_address; 
                        break;
                    }
                    case ROOM_K_BASE: {
                        memcpy(&base, 
                               items[x].buffer_address, 
                               items[x].buffer_length);
                        if (base < 1) base = 1;
                        break;
                    }
                }

            }

        }

        /* initilize our base klass here */

        object_set_error1(object, OK);

        /* initialize our derived klass here */

        self = ROOM(object);

        /* assign our methods here */

        self->ctor = _room_ctor;
        self->dtor = _room_dtor;
        self->_compare = _room_compare;
        self->_override = _room_override;

        self->_open  = _room_open;
        self->_close = _room_close;
        self->_add   = _room_add;
        self->_next  = _room_next;
        self->_prev  = _room_prev;
        self->_last  = _room_last;
        self->_first = _room_first;
        self->_get   = _room_get;
        self->_put   = _room_put;
        self->_read  = _room_read;
        self->_write = _room_write;
        self->_build = _room_build;

        /* initialize internal variables here */

        when_error_in {

            strncpy(roombase, fnm_build(1, FnmPath, "rooms", ".dat", database, NULL), 255);
            self->rooms = files_create(roombase, retries, timeout);
            check_creation(self->rooms);

            self->jam = NULL;
            self->base = base;
            self->trace = dump;
            self->retries = retries;
            self->timeout = timeout;
            self->msgbase = strdup(msgbase);

            exit_when;

        } use {

            stat = ERR;
            process_error(self);

        } end_when;

    }

    return stat;

}

int _room_dtor(object_t *object) {

    int stat = OK;
    room_t *self = ROOM(object);

    /* free local resources here */

    if (self->jam != NULL) {

        jam_close(self->jam);
        jam_destroy(self->jam);

    }

    free(self->msgbase);
    files_close(self->rooms);

    /* walk the chain, freeing as we go */

    object_demote(object, object_t);
    object_destroy(object);

    return stat;

}

int _room_override(room_t *self, item_list_t *items) {

    int stat = ERR;

    if (items != NULL) {

        int x;
        for (x = 0;; x++) {

            if ((items[x].buffer_length == 0) &&
                (items[x].item_code == 0)) break;

            switch(items[x].item_code) {
                case ROOM_M_DESTRUCTOR: {
                    self->dtor = items[x].buffer_address;
                    stat = OK;
                    break;
                }
            }

        }

    }

    return stat;

}

int _room_compare(room_t *self, room_t *other) {

    int stat = ERR;

    if ((object_compare(OBJECT(self), OBJECT(other)) == 0) &&
        (self->ctor == other->ctor) &&
        (self->dtor == other->dtor) &&
        (self->_compare == other->_compare) &&
        (self->_override == other->_override) &&
        (self->_open  == other->_open) &&
        (self->_close == other->_close) &&
        (self->_add   == other->_add) &&
        (self->_next  == other->_next) &&
        (self->_prev  == other->_prev) &&
        (self->_last  == other->_last) &&
        (self->_first == other->_first) &&
        (self->_get   == other->_get) &&
        (self->_put   == other->_put) &&
        (self->_read  == other->_read) &&
        (self->_write == other->_write) &&
        (self->_build == other->_build) &&
        (self->base == other->base) &&
        (self->retries == other->retries) &&
        (self->timeout == other->timeout) &&
        (self->msgbase == other->msgbase) &&
        (self->jam == other->jam) &&
        (self->rooms = other->rooms) &&
        (self->trace == other->trace)) {

        stat = OK;

    }

    return stat;

}

int _room_open(room_t *self) {

    int stat = OK;
    int exists = 0;
    room_base_t aide;
    room_base_t email;
    room_base_t lobby;
    int flags = O_RDWR;
    int create = (O_RDWR | O_CREAT);
    int mode = (S_IRWXU | S_IRWXG);

    when_error_in {

        stat = files_exists(self->rooms, &exists);
        check_return(stat, self->rooms);

        if (exists) {

            stat = files_open(self->rooms, flags, 0);
            check_return(stat, self->rooms);

        } else {

            stat = files_open(self->rooms, create, mode);
            check_return(stat, self->rooms);

            memset(&aide, '\0', sizeof(room_base_t));
            memset(&email, '\0', sizeof(room_base_t));
            memset(&lobby, '\0', sizeof(room_base_t));

            email.conference = 0;
            email.base = self->base;
            strcpy(email.name, "Mail");
            email.retries = self->retries;
            email.timeout = self->timeout;
            email.flags = (PERMROOM | PUBLIC | INUSE);
            strncpy(email.path, fnm_build(1, FnmPath, self->msgbase, NULL), 255);

            stat = self->_add(self, &email);
            check_return(stat, self);

            lobby.conference = 1;
            lobby.base = self->base;
            strcpy(lobby.name, "Lobby");
            lobby.retries = self->retries;
            lobby.timeout = self->timeout;
            lobby.flags = (PERMROOM | PUBLIC | INUSE);
            strncpy(lobby.path, fnm_build(1, FnmPath, self->msgbase, NULL), 255);

            stat = self->_add(self, &lobby);
            check_return(stat, self);

            aide.conference = 2;
            aide.base = self->base;
            strcpy(aide.name, "Aide");
            aide.retries = self->retries;
            aide.timeout = self->timeout;
            aide.flags = (PERMROOM | INUSE);
            strncpy(aide.path, fnm_build(1, FnmPath, self->msgbase, NULL), 255);

            stat = self->_add(self, &aide);
            check_return(stat, self);

        }

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int _room_close(room_t *self) {

    int stat = OK;

    when_error_in {

        if (self->jam != NULL) {

            stat = jam_close(self->jam);
            check_return(stat, self->jam);

            stat = jam_destroy(self->jam);
            check_return(stat, self->jam);

            self->jam = NULL;

        }

        stat = files_close(self->rooms);
        check_return(stat, self->rooms);

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int _room_first(room_t *self, room_base_t *room, ssize_t *count) {

    int stat = OK;
    room_base_t ondisk;

    when_error_in {

        stat = files_seek(self->rooms, 0, SEEK_SET);
        check_return(stat, self->rooms);

        stat = self->_read(self, &ondisk, count);
        check_return(stat, self);

        if (*count == sizeof(room_base_t)) {

            stat = self->_build(self, &ondisk, room);
            check_return(stat, self);

        }

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int _room_next(room_t *self, room_base_t *room, ssize_t *count) {

    int stat = OK;
    room_base_t ondisk;

    when_error_in {

        stat = self->_read(self, &ondisk, count);
        check_return(stat, self);

        if (*count == sizeof(room_base_t )) {

            stat = self->_build(self, &ondisk, room);
            check_return(stat, self);

        }

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int _room_prev(room_t *self, room_base_t *room, ssize_t *count) {

    int stat = OK;
    room_base_t ondisk;
    ssize_t position = 0;
    off_t offset = sizeof(room_base_t);

    when_error_in {

        stat = files_tell(self->rooms, &position);
        check_return(stat, self->rooms);

        if (position > 0) {

            stat = files_seek(self->rooms, -offset, SEEK_CUR);
            check_return(stat, self->rooms);

            stat = self->_read(self, &ondisk, count);
            check_return(stat, self);

            stat = files_seek(self->rooms, -offset, SEEK_CUR);
            check_return(stat, self->rooms);

            if (*count == sizeof(room_base_t)) {

                stat = self->_build(self, &ondisk, room);
                check_return(stat, self);

            }

        } else {

            *count = 0;

        }

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int _room_last(room_t *self, room_base_t *room, ssize_t *count) {

    int stat = OK;
    room_base_t ondisk;
    off_t offset = sizeof(room_base_t);

    when_error_in {

        stat = files_seek(self->rooms, 0, SEEK_END);
        check_return(stat, self->rooms);

        stat = files_seek(self->rooms, -offset, SEEK_CUR);
        check_return(stat, self->rooms);

        stat = self->_read(self, &ondisk, count);
        check_return(stat, self);

        stat = files_seek(self->rooms, -offset, SEEK_CUR);
        check_return(stat, self->rooms);

        if (*count == sizeof(room_base_t)) {

            stat = self->_build(self, &ondisk, room);
            check_return(stat, self);

        }

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int _room_add(room_t *self, room_base_t *room) {

    int stat = OK;
    ssize_t count = 0;

    when_error_in {

        stat = files_seek(self->rooms, 0, SEEK_END);
        check_return(stat, self->rooms);

        stat = self->_write(self, room, &count);
        check_return(stat, self);

        if (count != sizeof(room_base_t)) {

            cause_error(EIO);

        }

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int _room_get(room_t *self, short conference, room_base_t *room) {

    int stat = OK;
    ssize_t count = 0;
    room_base_t ondisk;

    when_error_in {

        stat = files_seek(self->rooms, 0, SEEK_SET);
        check_return(stat, self->rooms);

        stat = self->_read(self, &ondisk, &count);
        check_return(stat, self);

        while (count > 0) {

            if (ondisk.conference == conference) {

                self->_build(self, &ondisk, room);
                break;

            }

            stat = self->_read(self, &ondisk, &count);
            check_return(stat, self);

        }

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int _room_put(room_t *self, short conference, room_base_t *room) {

    int stat = OK;
    ssize_t count = 0;
    room_base_t ondisk;
    off_t offset = sizeof(room_base_t);

    when_error_in {

        stat = files_seek(self->rooms, 0, SEEK_SET);
        check_return(stat, self->rooms);

        stat = self->_read(self, &ondisk, &count);
        check_return(stat, self);

        while (count > 0) {

            if (ondisk.conference == conference) {

                ondisk.base = room->base;
                ondisk.flags = room->flags;
                ondisk.retries = room->retries;
                ondisk.timeout = room->timeout;
                ondisk.conference = room->conference;
                strncpy(ondisk.name, room->name, 32);
                strncpy(ondisk.path, room->path, 256);

                stat = files_seek(self->rooms, -offset, SEEK_CUR);
                check_return(stat, self->rooms);

                stat = self->_write(self, &ondisk, &count);
                check_return(stat, self);

                if (count != sizeof(room_base_t)) {

                    cause_error(EIO);

                }

                break;

            }

            stat = self->_read(self, &ondisk, &count);
            check_return(stat, self);

        }

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int _room_build(room_t *self, room_base_t *ondisk, room_base_t *room) {

    char name[7];
    int stat = OK;

    when_error_in {

        (*room).base = ondisk->base;
        (*room).flags = ondisk->flags;
        (*room).retries = ondisk->retries;
        (*room).timeout = ondisk->timeout;
        (*room).conference = ondisk->conference;

        memset((*room).name, '\0', 32);
        strncpy((*room).name, ondisk->name, 31);

        memset((*room).path, '\0', 256);
        strncpy((*room).path, ondisk->path, 255);

        if (self->jam != NULL) {

            stat = jam_close(self->jam);
            check_return(stat, self->jam);

            stat = jam_destroy(self->jam);
            check_return(stat, self->jam);

            self->jam = NULL;

        }

        memset(name, '\0', 7);
        snprintf(name, 6, "%05d", (int)room->conference);

        self->jam = jam_create(self->msgbase, name, room->retries, room->timeout, room->base, self->trace);
        check_creation(self->jam);

        stat = jam_open(self->jam);
        check_return(stat, self->jam);

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int _room_read(room_t *self, room_base_t *room, ssize_t *count) {

    int stat = OK;

    when_error_in {

        stat = files_read(self->rooms, room, sizeof(room_base_t), count);
        check_return(stat, self->rooms);

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int _room_write(room_t *self, room_base_t *room, ssize_t *count) {

    int stat = OK;

    when_error_in {

        stat = files_write(self->rooms, room, sizeof(room_base_t), count);
        check_return(stat, self->rooms);

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

