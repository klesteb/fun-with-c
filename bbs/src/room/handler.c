
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

#include "files/files.h"
#include "include/when.h"
#include "objects/object.h"
#include "include/error_codes.h"

#include "bbs/src/room/handler.h"

require_klass(OBJECT_KLASS);

/*----------------------------------------------------------------*/
/* klass methods                                                  */
/*----------------------------------------------------------------*/

int _handler_ctor(object_t *, item_list_t *);
int _handler_dtor(object_t *);
int _handler_compare(handler_t *, handler_t *);
int _handler_override(handler_t *, item_list_t *);

int _handler_init(handler_t *);
int _handler_detach(handler_t *);
int _handler_remove(handler_t *);
int _handler_handle(handler_t *, void **);
int _handler_attach(handler_t *, room_base_t *);

/*----------------------------------------------------------------*/
/* klass declaration                                              */
/*----------------------------------------------------------------*/

declare_klass(HANLDER_KLASS) {
    .size = KLASS_SIZE(handler_t),
    .name = KLASS_NAME(handler_t),
    .ctor = _handler_ctor,
    .dtor = _handler_dtor,
};

/*----------------------------------------------------------------*/
/* klass interface                                                */
/*----------------------------------------------------------------*/

handler_t *handler_create(files_t *db, char *path, int retries, int timeout, int base, tracer_t *dump) {

    int stat = ERR;
    item_list_t items[7];
    handler_t *self = NULL;

    SET_ITEM(items[0], HANDLER_K_DB, db, 0, NULL);
    SET_ITEM(items[1], HANDLER_K_PATH, path, strlen(path), NULL);
    SET_ITEM(items[2], HANDLER_K_RETRIES, &retries, sizeof(int), NULL);
    SET_ITEM(items[3], HANDLER_K_TIMEOUT, &timeout, sizeof(int), NULL);
    SET_ITEM(items[4], HANDLER_K_BASE, &base, sizeof(int), NULL);
    SET_ITEM(items[5], HANDLER_K_TRACE, dump, 0, NULL);
    SET_ITEM(items[6], 0,0,0,0);

    self = (handler_t *)object_create(HANLDER_KLASS, items, &stat);

    return self;

}

int handler_destroy(handler_t *self) {

    int stat = OK;

    when_error {

        if (self != NULL) {

            if (object_assert(self, handler_t)) {

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

int handler_override(handler_t *self, item_list_t *items) {

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

int handler_compare(handler_t *us, handler_t *them) {

    int stat = OK;

    when_error {

        if (us != NULL) {

            if (object_assert(them, handler_t)) {

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

char *handler_version(handler_t *self) {

    char *version = VERSION;

    return version;

}

int handler_init(handler_t *self) {

    int stat = OK;

    when_error_in {

        if (self == NULL) {

            cause_error(E_INVPARM);

        }

        stat = self->_init(self);
        check_return(stat, self);

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int handler_attach(handler_t *self, room_base_t *room) {

    int stat = OK;

    when_error_in {

        if ((self == NULL) || (room == NULL)) {

            cause_error(E_INVPARM);

        }

        stat = self->_attach(self, room);
        check_return(stat, self);

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int handler_detach(handler_t *self) {

    int stat = OK;

    when_error_in {

        if (self == NULL) {

            cause_error(E_INVPARM);

        }

        stat = self->_detach(self);
        check_return(stat, self);

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int handler_remove(handler_t *self) {

    int stat = OK;

    when_error_in {

        if (self == NULL) {

            cause_error(E_INVPARM);

        }

        stat = self->_remove(self);
        check_return(stat, self);

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int handler_handle(handler_t *self, void **handle) {

    int stat = OK;

    when_error_in {

        if (self == NULL) {

            cause_error(E_INVPARM);

        }

        stat = self->_handle(self, handle);
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

int _handler_ctor(object_t *object, item_list_t *items) {

    int base = 0;
    char path[256];
    int stat = ERR;
    int timeout = 1;
    int retries = 30;
    files_t *db = NULL;
    tracer_t *dump = NULL;
    handler_t *self = NULL;

    if (object != NULL) {

        memset(path, '\0', 256);

        /* capture our items */

        if (items != NULL) {

            int x;
            for (x = 0;; x++) {

                if ((items[x].buffer_length == 0) &&
                    (items[x].item_code == 0)) break;

                switch(items[x].item_code) {
                    case HANDLER_K_TRACE: {
                        dump = items[x].buffer_address; 
                        break;
                    }
                    case HANDLER_K_DB: {
                        db = items[x].buffer_address; 
                        break;
                    }
                    case HANDLER_K_PATH: {
                        memcpy(&path, 
                               items[x].buffer_address, 
                               items[x].buffer_length);
                        break;
                    }
                    case HANDLER_K_RETRIES: {
                        memcpy(&retries, 
                               items[x].buffer_address, 
                               items[x].buffer_length);
                        break;
                    }
                    case HANDLER_K_TIMEOUT: {
                        memcpy(&timeout, 
                               items[x].buffer_address, 
                               items[x].buffer_length);
                        break;
                    }
                    case HANDLER_K_BASE: {
                        memcpy(&base, 
                               items[x].buffer_address, 
                               items[x].buffer_length);
                        break;
                    }
                 }

            }

        }

        /* initilize our base klass here */

        object_set_error1(object, OK);

        /* initialize our derived klass here */

        self = HANDLER(object);

        /* assign our methods here */

        self->ctor = _handler_ctor;
        self->dtor = _handler_dtor;
        self->_compare = _handler_compare;
        self->_override = _handler_override;

        self->_init = _handler_init;
        self->_attach = _handler_attach;
        self->_detach = _handler_detach;
        self->_remove = _handler_remove;
        self->_handle = _handler_handle;

        /* initialize internal variables here */
        
        self->db = db;
        self->base = base;
        self->trace = dump;
        self->handle = NULL;
        self->timeout = timeout;
        self->retries = retries;
        self->path = strdup(path);

        stat = OK;

    }

    return stat;

}

int _handler_dtor(object_t *object) {

    int stat = OK;
    handler_t *self = HANDLER(object);

    /* free local resources here */

    free(self->path);

    /* walk the chain, freeing as we go */

    object_demote(object, object_t);
    object_destroy(object);

    return stat;

}

int _handler_override(handler_t *self, item_list_t *items) {

    int stat = ERR;

    if (items != NULL) {

        int x;
        for (x = 0;; x++) {

            if ((items[x].buffer_length == 0) &&
                (items[x].item_code == 0)) break;

            switch(items[x].item_code) {
                case HANDLER_M_DESTRUCTOR: {
                    self->dtor = items[x].buffer_address;
                    stat = OK;
                    break;
                }
                case HANDLER_M_INIT: {
                    self->_init = items[x].buffer_address;
                    stat = OK;
                    break;
                }
                case HANDLER_M_ATTACH: {
                    self->_attach = items[x].buffer_address;
                    stat = OK;
                    break;
                }
                case HANDLER_M_DETACH: {
                    self->_detach = items[x].buffer_address;
                    stat = OK;
                    break;
                }
                case HANDLER_M_REMOVE: {
                    self->_remove = items[x].buffer_address;
                    stat = OK;
                    break;
                }
                case HANDLER_M_HANDLE: {
                    self->_handle = items[x].buffer_address;
                    stat = OK;
                    break;
                }
            }

        }

    }

    return stat;

}

int _handler_compare(handler_t *self, handler_t *other) {

    int stat = ERR;

    if ((object_compare(OBJECT(self), OBJECT(other)) == 0) &&
        (self->ctor == other->ctor) &&
        (self->dtor == other->dtor) &&
        (self->_compare == other->_compare) &&
        (self->_override == other->_override) &&
        (self->_init == other->_init) &&
        (self->_attach == other->_attach) &&
        (self->_detach == other->_detach) &&
        (self->_remove == other->_remove)) {

        stat = OK;

    }

    return stat;

}

int _handler_init(handler_t *self) {

    return OK;

}

int _handler_attach(handler_t *self, room_base_t *room) {

    return OK;

}

int _handler_detach(handler_t *self) {

    return OK;

}

int _handler_remove(handler_t *self) {

    return OK;

}

int _handler_handle(handler_t *self, void **handle) {

    *handle = NULL;
    return OK;
    
}

