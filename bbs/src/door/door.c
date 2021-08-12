
/*---------------------------------------------------------------------------*/
/*                Copyright (c) 2019 by Kevin L. Esteb                       */
/*                                                                           */
/*  Permission to use, copy, modify, and distribute this software and its    */
/*  documentation for any purpose and without fee is hereby granted,         */
/*  provided that this copyright notice appears in all copies. The author    */
/*  makes no representations about the suitability of this software for      */
/*  any purpose. It is provided "as is" without express or implied warranty. */
/*                                                                           */
/*---------------------------------------------------------------------------*/

#include "include/when.h"
#include "include/error_codes.h"

#include "objects/object.h"

#include "bbs/src/door/door.h"
#include "bbs/src/main/bbs_config.h"

require_klass(OBJECT_KLASS);

/*----------------------------------------------------------------*/
/* klass methods                                                  */
/*----------------------------------------------------------------*/

int _door_ctor(object_t *, item_list_t *);
int _door_dtor(object_t *);

int _door_compare(door_t *, door_t *);
int _door_override(door_t *, item_list_t *);

/*----------------------------------------------------------------*/
/* klass declaration                                              */
/*----------------------------------------------------------------*/

declare_klass(DOOR_KLASS) {
    .size = KLASS_SIZE(door_t),
    .name = KLASS_NAME(door_t),
    .ctor = _door_ctor,
    .dtor = _door_dtor,
};

/*----------------------------------------------------------------*/
/* klass interface                                                */
/*----------------------------------------------------------------*/

door_t *door_create(char *path, int retries, int timeout, tracer_t *dump) {

    int stat = ERR;
    door_t *self = NULL;
    item_list_t items[5];

    SET_ITEM(items[0], DOOR_K_PATH, path, strlen(path), NULL);
    SET_ITEM(items[1], DOOR_K_RETRIES, &retries, sizeof(int), NULL);
    SET_ITEM(items[2], DOOR_K_TIMEOUT, &timeout, sizeof(int), NULL);
    SET_ITEM(items[3], DOOR_K_TRACE, dump, 0, NULL);
    SET_ITEM(items[4], 0, 0, 0, 0);

    self = (door_t *)object_create(DOOR_KLASS, items, &stat);

    return self;

}

int door_destroy(door_t *self) {

    int stat = OK;

    when_error {

        if (self != NULL) {

            if (object_assert(self, door_t)) {

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

        object_set_error2(self, trace_errnum, trace_lineno, trace_filename, trace_function);
        clear_error();

    } end_when;

    return stat;

}

int door_override(door_t *self, item_list_t *items) {

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

        object_set_error2(self, trace_errnum, trace_lineno, trace_filename, trace_function);
        clear_error();

    } end_when;

    return stat;

}

int door_compare(door_t *us, door_t *them) {

    int stat = OK;

    when_error {

        if (us != NULL) {

            if (object_assert(them, door_t)) {

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

        object_set_error2(us, trace_errnum, trace_lineno, trace_filename, trace_function);
        clear_error();

    } end_when;

    return stat;

}

char *door_version(door_t *self) {

    char *version = VERSION;

    return version;

}

/*----------------------------------------------------------------*/
/* klass implementation                                           */
/*----------------------------------------------------------------*/

int _door_ctor(object_t *object, item_list_t *items) {

    char path[256];
    int stat = ERR;
    door_t *self = NULL;

    if (object != NULL) {

        memset(path, '\0', 256);

        /* capture our items */

        if (items != NULL) {

            int x = 0;
            for (;; x++) {

                if ((items[x].buffer_length == 0) &&
                    (items[x].item_code == 0)) break;

                switch(items[x].item_code) {
                    case DOOR_K_PATH: {
                        memcpy(path, 
                               items[x].buffer_address, 
                               items[x].buffer_length);
                        break;
                    }
                    case DOOR_K_TIMEOUT: {
                        memcpy(&timeout, 
                               items[x].buffer_address, 
                               items[x].buffer_length);
                        break;
                    }
                    case DOOR_K_RETRIES: {
                        memcpy(&retries, 
                               items[x].buffer_address, 
                               items[x].buffer_length);
                        break;
                    }
                    case DOOR_K_TRACE: {
                        dump = items[x].buffer_address; 
                        break;
                    }
                }

            }

        }

        /* initilize our base klass here */

        object_set_error1(object, OK);

        /* initialize our derived klass here */

        self = DOOR(object);

        /* assign our methods here */

        self->ctor      = _door_ctor;
        self->dtor      = _door_dtor;
        self->_compare  = _door_compare;
        self->_override = _door_override;

        /* initialize internal variables here */

        when_error_in {

            self->trace = dump;
            self->retries = retries;
            self->timeout = timeout;
            self->path = strdup(path);

            strncpy(control, fnm_build(1, FnmPath, "door", ".ctl", path, NULL), 255);
            self->control = files_create(control, retries, timeout);
            check_creation(self->control);

            exit_when;

        } use {

            stat = ERR;
            process_error(self);

        } end_when;

    }

    return stat;

}

int _door_dtor(object_t *object) {

    int stat = OK;
    door_t *self = DOOR(object);

    /* free local resources here */

    free(self->path);

    file_close(self->control);
    file_destroy(self->control);

    /* walk the chain, freeing as we go */

    object_demote(object, object_t);
    object_destroy(object);

    return stat;

}

int _door_override(door_t *self, item_list_t *items) {

    int stat = ERR;

    if (items != NULL) {

        int x;
        for (x = 0;; x++) {

            if ((items[x].buffer_length == 0) &&
                (items[x].item_code == 0)) break;

            switch(items[x].item_code) {
                case DOOR_M_DESTRUCTOR: {
                    self->dtor = items[x].buffer_address;
                    stat = OK;
                    break;
                }
            }

        }

    }

    return stat;

}

int _door_compare(door_t *this, door_t *that) {

    int stat = ERR;

    if ((object_compare(OBJECT(this), OBJECT(that)) == 0) &&
        (this->ctor == that->ctor) &&
        (this->dtor == that->dtor) &&
        (this->_compare == that->_compare) &&
        (this->_override == that->_override)) {

        stat = OK;

    }

    return stat;

}

int _door_open(door_t *door) {
    
    int stat = OK;
    int exists = 0;
    int flags = O_RDWR;

    when_error_in {

        stat = files_exists(self->control, &exists);
        check_return(stat, self->control);

        if (exists) {

            stat = files_open(self->control, flags, 0);
            check_return(stat, self->control);


        }

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int _door_close(door_t *door) {

    int stat = OK;

    when_error_in {

        stat = files_close(self->control);
        check_return(stat, self->control);

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int _door_run(door_t *door) {

    int stat = OK;

    when_error_in {


        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

