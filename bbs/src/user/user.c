
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

#include "user.h"
#include "when.h"
#include "files.h"
#include "object.h"
#include "tracer.h"
#include "fnm_util.h"
#include "error_codes.h"

require_klass(OBJECT_KLASS);

/*----------------------------------------------------------------*/
/* klass methods                                                  */
/*----------------------------------------------------------------*/

int _user_ctor(object_t *, item_list_t *);
int _user_dtor(object_t *);
int _user_compare(user_t *, user_t *);
int _user_override(user_t *, item_list_t *);

int _user_open(user_t *);
int _user_close(user_t *);
int _user_unlock(user_t *);
int _user_del(user_t *, long);
int _user_extend(user_t *, int);
int _user_lock(user_t *, off_t);
int _user_add(user_t *, user_base_t *);
int _user_get_sequence(user_t *, long *);
int _user_get(user_t *, int, user_base_t *);
int _user_put(user_t *, int, user_base_t *);
int _user_next(user_t *, user_base_t *, ssize_t *);
int _user_prev(user_t *, user_base_t *, ssize_t *);
int _user_last(user_t *, user_base_t *, ssize_t *);
int _user_read(user_t *, user_base_t *, ssize_t *);
int _user_write(user_t *, user_base_t *, ssize_t *);
int _user_first(user_t *, user_base_t *, ssize_t *);
int _user_build(user_t *, user_base_t *, user_base_t *);

/*----------------------------------------------------------------*/
/* klass declaration                                              */
/*----------------------------------------------------------------*/

declare_klass(USER_KLASS) {
    .size = KLASS_SIZE(user_t),
    .name = KLASS_NAME(user_t),
    .ctor = _user_ctor,
    .dtor = _user_dtor,
};

/*----------------------------------------------------------------*/
/* klass macros                                                   */
/*----------------------------------------------------------------*/

#define USER_OFFSET(n)   ((((n) - 1) * sizeof(user_base_t)))
#define USER_RECORD(n)   (((n) / sizeof(user_base_t)) + 1)

/*----------------------------------------------------------------*/
/* klass interface                                                */
/*----------------------------------------------------------------*/

user_t *user_create(char *path, int users, int retries, int timeout, tracer_t *dump) {

    int stat = ERR;
    user_t *self = NULL;
    item_list_t items[6];

    SET_ITEM(items[0], USER_K_PATH, path, strlen(path), NULL);
    SET_ITEM(items[1], USER_K_RETRIES, &retries, sizeof(int), NULL);
    SET_ITEM(items[2], USER_K_TIMEOUT, &timeout, sizeof(int), NULL);
    SET_ITEM(items[3], USER_K_USERS, &users, sizeof(int), NULL);
    SET_ITEM(items[4], USER_K_TRACE, dump, 0, NULL);
    SET_ITEM(items[5], 0,0,0,0);

    self = (user_t *)object_create(USER_KLASS, items, &stat);

    return self;

}

int user_destroy(user_t *self) {

    int stat = OK;

    when_error {

        if (self != NULL) {

            if (object_assert(self, user_t)) {

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

int user_override(user_t *self, item_list_t *items) {

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

int user_compare(user_t *us, user_t *them) {

    int stat = OK;

    when_error {

        if (us != NULL) {

            if (object_assert(them, user_t)) {

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

char *user_version(user_t *self) {

    char *version = VERSION;

    return version;

}

int user_open(user_t *self) {

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

int user_close(user_t *self) {

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

int user_first(user_t *self, user_base_t *user, ssize_t *count) {

    int stat = OK;

    when_error_in {

        if ((self == NULL) || (user == NULL)) {

            cause_error(E_INVPARM);

        }

        stat = self->_first(self, user, count);
        check_return(stat, self);

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int user_next(user_t *self, user_base_t *user, ssize_t *count) {

    int stat = OK;

    when_error_in {

        if ((self == NULL) || (user == NULL)) {

            cause_error(E_INVPARM);

        }

        stat = self->_next(self, user, count);
        check_return(stat, self);

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int user_prev(user_t *self, user_base_t *user, ssize_t *count) {

    int stat = OK;

    when_error_in {

        if ((self == NULL) || (user == NULL)) {

            cause_error(E_INVPARM);

        }

        stat = self->_prev(self, user, count);
        check_return(stat, self);

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int user_last(user_t *self, user_base_t *user, ssize_t *count) {

    int stat = OK;

    when_error_in {

        if ((self == NULL) || (user == NULL)) {

            cause_error(E_INVPARM);

        }

        stat = self->_last(self, user, count);
        check_return(stat, self);

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int user_add(user_t *self, user_base_t *user) {

    int stat = OK;

    when_error_in {

        if ((self == NULL) || (user == NULL)) {

            cause_error(E_INVPARM);

        }

        stat = self->_add(self, user);
        check_return(stat, self);

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int user_del(user_t *self, ushort userno) {

    int stat = OK;

    when_error_in {

        if (self == NULL) {

            cause_error(E_INVPARM);

        }

        stat = self->_del(self, userno);
        check_return(stat, self);

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int user_get(user_t *self, int index, user_base_t *user) {

    int stat = OK;

    when_error_in {

        if ((self == NULL) || (user == NULL) ||
            ((index < 0) && (index > self->users))) {

            cause_error(E_INVPARM);

        }

        stat = self->_get(self, index, user);
        check_return(stat, self);

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int user_put(user_t *self, int index, user_base_t *user) {

    int stat = OK;

    when_error_in {

        if ((self == NULL) || (user == NULL) ||
            ((index < 0) && (index > self->users))) {

            cause_error(E_INVPARM);

        }

        stat = self->_put(self, index, user);
        check_return(stat, self);

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int user_index(user_t *self, int *index) {

    int stat = OK;

    when_error_in {

        if ((self == NULL) || (index == NULL)) {

            cause_error(E_INVPARM);

        }

        *index = self->index;

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

int _user_ctor(object_t *object, item_list_t *items) {

    int stat = OK;
    int users = 2;
    char seq[256];
    char path[256];
    char prof[256];
    int timeout = 1;
    char userdb[256];
    int retries = 30;
    user_t *self = NULL;
    tracer_t *dump = NULL;

    if (object != NULL) {

        memset(path, '\0', 256);
        memset(prof, '\0', 256);
        memset(userdb, '\0', 256);

        /* capture our items */

        if (items != NULL) {

            int x;
            for (x = 0;; x++) {

                if ((items[x].buffer_length == 0) &&
                    (items[x].item_code == 0)) break;

                switch(items[x].item_code) {
                    case USER_K_PATH: {
                        memcpy(&path, 
                               items[x].buffer_address, 
                               items[x].buffer_length);
                        break;
                    }
                    case USER_K_RETRIES: {
                        memcpy(&retries, 
                               items[x].buffer_address, 
                               items[x].buffer_length);
                        break;
                    }
                    case USER_K_TIMEOUT: {
                        memcpy(&timeout, 
                               items[x].buffer_address, 
                               items[x].buffer_length);
                        break;
                    }
                    case USER_K_USERS: {
                        memcpy(&users, 
                               items[x].buffer_address, 
                               items[x].buffer_length);
                        break;
                    }
                    case USER_K_TRACE: {
                        dump = items[x].buffer_address; 
                        break;
                    }
                }

            }

        }

        /* initilize our base klass here */

        object_set_error1(object, OK);

        /* initialize our derived klass here */

        self = USER(object);

        /* assign our methods here */

        self->ctor = _user_ctor;
        self->dtor = _user_dtor;
        self->_compare = _user_compare;
        self->_override = _user_override;

        self->_add    = _user_add;
        self->_get    = _user_get;
        self->_del    = _user_del;
        self->_put    = _user_put;
        self->_open   = _user_open;
        self->_lock   = _user_lock;
        self->_next   = _user_next;
        self->_prev   = _user_prev;
        self->_last   = _user_last;
        self->_read   = _user_read;
        self->_write  = _user_write;
        self->_first  = _user_first;
        self->_build  = _user_build;
        self->_close  = _user_close;
        self->_unlock = _user_unlock;
        self->_extend = _user_extend;
        self->_get_sequence = _user_get_sequence;

        /* initialize internal variables here */

        when_error_in {

            self->index = 0;
            self->trace = dump;
            self->users = users;
            self->locked = FALSE;
            self->retries = retries;
            self->timeout = timeout;
            self->path = strdup(path);

            strncpy(userdb, fnm_build(1, FnmPath, "users", ".dat", path, NULL), 255);
            self->userdb = files_create(userdb, retries, timeout);
            check_creation(self->userdb);

            strncpy(prof, fnm_build(1, FnmPath, "profiles", ".dat", path, NULL), 255);
            self->profiles = files_create(prof, retries, timeout);
            check_creation(self->profiles);

            strncpy(seq, fnm_build(1, FnmPath, "users", ".seq", path, NULL), 255);
            self->sequence = files_create(seq, retries, timeout);
            check_creation(self->sequence);

            exit_when;

        } use {

            stat = ERR;
            process_error(self);

        } end_when;

    }

    return stat;

}

int _user_dtor(object_t *object) {

    int stat = OK;
    user_t *self = USER(object);

    /* free local resources here */

    files_close(self->userdb);
    files_close(self->sequence);
    free(self->path);

    /* walk the chain, freeing as we go */

    object_demote(object, object_t);
    object_destroy(object);

    return stat;

}

int _user_override(user_t *self, item_list_t *items) {

    int stat = ERR;

    if (items != NULL) {

        int x;
        for (x = 0;; x++) {

            if ((items[x].buffer_length == 0) &&
                (items[x].item_code == 0)) break;

            switch(items[x].item_code) {
                case USER_M_DESTRUCTOR: {
                    self->dtor = items[x].buffer_address;
                    stat = OK;
                    break;
                }
                case USER_M_OPEN: {
                    self->_open = items[x].buffer_address;
                    stat = OK;
                    break;
                }
                case USER_M_CLOSE: {
                    self->_close = items[x].buffer_address;
                    stat = OK;
                    break;
                }
                case USER_M_UNLOCK: {
                    self->_unlock = items[x].buffer_address;
                    stat = OK;
                    break;
                }
                case USER_M_LOCK: {
                    self->_lock = items[x].buffer_address;
                    stat = OK;
                    break;
                }
                case USER_M_GET: {
                    self->_get = items[x].buffer_address;
                    stat = OK;
                    break;
                }
                case USER_M_PUT: {
                    self->_put = items[x].buffer_address;
                    stat = OK;
                    break;
                }
                case USER_M_NEXT: {
                    self->_next = items[x].buffer_address;
                    stat = OK;
                    break;
                }
                case USER_M_PREV: {
                    self->_prev = items[x].buffer_address;
                    stat = OK;
                    break;
                }
                case USER_M_LAST: {
                    self->_last = items[x].buffer_address;
                    stat = OK;
                    break;
                }
                case USER_M_WRITE: {
                    self->_write = items[x].buffer_address;
                    stat = OK;
                    break;
                }
                case USER_M_FIRST: {
                    self->_first = items[x].buffer_address;
                    stat = OK;
                    break;
                }
                case USER_M_BUILD: {
                    self->_build = items[x].buffer_address;
                    stat = OK;
                    break;
                }
                case USER_M_EXTEND: {
                    self->_extend = items[x].buffer_address;
                    stat = OK;
                    break;
                }
                case USER_M_ADD: {
                    self->_add = items[x].buffer_address;
                    stat = OK;
                    break;
                }
                case USER_M_DEL: {
                    self->_del = items[x].buffer_address;
                    stat = OK;
                    break;
                }
            }

        }

    }

    return stat;

}

int _user_compare(user_t *self, user_t *other) {

    int stat = ERR;

    if ((object_compare(OBJECT(self), OBJECT(other)) == 0) &&
        (self->ctor == other->ctor) &&
        (self->dtor == other->dtor) &&
        (self->_compare == other->_compare) &&
        (self->_override == other->_override) &&
        (self->_add    == other->_add) &&
        (self->_del    == other->_del) &&
        (self->_get    == other->_get) &&
        (self->_put    == other->_put) &&
        (self->_open   == other->_open) &&
        (self->_lock   == other->_lock) &&
        (self->_next   == other->_next) &&
        (self->_prev   == other->_prev) &&
        (self->_last   == other->_last) &&
        (self->_read   == other->_read) &&
        (self->_write  == other->_write) &&
        (self->_first  == other->_first) &&
        (self->_build  == other->_build) &&
        (self->_close  == other->_close) &&
        (self->_unlock == other->_unlock) &&
        (self->_extend == other->_extend) &&
        (self->locked == other->locked) &&
        (self->userdb == other->userdb) &&
        (self->trace  == other->trace)) {

        stat = OK;

    }

    return stat;

}

int _user_open(user_t *self) {

    int stat = OK;
    int exists = 0;
    ssize_t count = 0;
    long sequence = 1;
    user_base_t ondisk;
    int flags = O_RDWR;
    int mode = (S_IRWXU | S_IRWXG);
    int create = (O_RDWR | O_CREAT);

    when_error_in {

        stat = files_exists(self->sequence, &exists);
        check_return(stat, self->sequence);

        if (exists) {

            stat = files_open(self->sequence, flags, 0);
            check_return(stat, self->sequence);

        } else {

            stat = files_open(self->sequence, create, mode);
            check_return(stat, self->sequence);

            stat = files_write(self->sequence, &sequence, sizeof(long), &count);
            check_return(stat, self->sequence);

            if (count != sizeof(long)) {

                cause_error(EIO);

            }

        }

        stat = files_exists(self->userdb, &exists);
        check_return(stat, self->userdb);

        if (exists) {

            stat = files_open(self->userdb, flags, 0);
            check_return(stat, self->userdb);

        } else {

            stat = files_open(self->userdb, create, mode);
            check_return(stat, self->userdb);

            stat = self->_extend(self, self->users);
            check_return(stat, self);

            /* create sysop account */

            ondisk.eternal = 1;
            ondisk.revision = 1;
            ondisk.axlevel = AX_SYSOP;
            ondisk.flags = (US_SYSOP | US_ROOMAIDE | US_PERM | US_SUBSYSTEM | US_NEEDVALID);
            memset(&ondisk.username, '\0', 26);
            strcpy(ondisk.username, "sysop");
            memset(&ondisk.password, '\0', 26);
            strcpy(ondisk.password, "sysop");

            self->_put(self, 1, &ondisk);
            check_return(stat, self);

            /* create network account */

            ondisk.eternal = 2;
            ondisk.axlevel = AX_NET;
            ondisk.flags = (US_ROOMAIDE | US_PERM | US_SUBSYSTEM | US_NEEDVALID);
            memset(&ondisk.username, '\0', 26);
            strcpy(ondisk.username, "qwknet");
            memset(&ondisk.password, '\0', 26);
            strcpy(ondisk.password, "qwknet");

            self->_put(self, 2, &ondisk);
            check_return(stat, self);

            /* guest account */

            ondisk.eternal = 3;
            ondisk.flags = US_PERM;
            ondisk.axlevel = AX_NORM;
            memset(&ondisk.username, '\0', 26);
            strcpy(ondisk.username, "guest");
            memset(&ondisk.password, '\0', 26);
            strcpy(ondisk.password, "guest");

            self->_put(self, 3, &ondisk);
            check_return(stat, self);
            
        }

        stat = files_exists(self->profiles, &exists);
        check_return(stat, self->profiles);

        if (exists) {

            stat = files_open(self->profiles, flags, 0);
            check_return(stat, self->profiles);

        } else {

            stat = files_open(self->profiles, create, mode);
            check_return(stat, self->profiles);

        }

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

        if (self->locked) self->_unlock(self);

    } end_when;

    return stat;

}

int _user_close(user_t *self) {

    int stat = OK;

    when_error_in {

        stat = files_close(self->userdb);
        check_return(stat, self->userdb);

        stat = files_close(self->sequence);
        check_return(stat, self->sequence);

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int _user_first(user_t *self, user_base_t *user, ssize_t *count) {

    int stat = OK;
    user_base_t ondisk;
    ssize_t position = 0;

    when_error_in {

        stat = files_seek(self->userdb, 0, SEEK_SET);
        check_return(stat, self->userdb);

        stat = files_tell(self->userdb, &position);
        check_return(stat, self->userdb);

        self->index = USER_RECORD(position);

        stat = self->_lock(self, 0);
        check_return(stat, self);

        stat = self->_read(self, &ondisk, count);
        check_return(stat, self);

        stat = self->_unlock(self);
        check_return(stat, self);

        if (*count == sizeof(user_base_t)) {

            stat = self->_build(self, &ondisk, user);
            check_return(stat, self);

        }

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

        if (self->locked) self->_unlock(self);

    } end_when;

    return stat;

}

int _user_next(user_t *self, user_base_t *user, ssize_t *count) {

    int stat = OK;
    user_base_t ondisk;
    ssize_t position = 0;

    when_error_in {

        stat = files_tell(self->userdb, &position);
        check_return(stat, self);

        self->index = USER_RECORD(position);

        stat = self->_lock(self, position);
        check_return(stat, self);

        stat = self->_read(self, &ondisk, count);
        check_return(stat, self);

        stat = self->_unlock(self);
        check_return(stat, self);

        if (*count == sizeof(user_base_t)) {

            stat = self->_build(self, &ondisk, user);
            check_return(stat, self);

        }

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

        if (self->locked) self->_unlock(self);

    } end_when;

    return stat;

}

int _user_prev(user_t *self, user_base_t *user, ssize_t *count) {

    int stat = OK;
    user_base_t ondisk;
    ssize_t position = 0;
    off_t offset = sizeof(user_base_t);

    when_error_in {

        stat = files_tell(self->userdb, &position);
        check_return(stat, self->userdb);

        if (position > 0) {

            stat = files_seek(self->userdb, -offset, SEEK_CUR);
            check_return(stat, self->userdb);

            stat = self->_lock(self, position - offset);
            check_return(stat, self);

            self->index = USER_RECORD(position - offset);

            stat = self->_read(self, &ondisk, count);
            check_return(stat, self);

            stat = self->_unlock(self);
            check_return(stat, self);

            stat = files_seek(self->userdb, -offset, SEEK_CUR);
            check_return(stat, self->userdb);

            if (*count == sizeof(user_base_t)) {

                stat = self->_build(self, &ondisk, user);
                check_return(stat, self);

            }

        } else {

            *count = 0;

        }

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

        if (self->locked) self->_unlock(self);

    } end_when;

    return stat;

}

int _user_last(user_t *self, user_base_t *user, ssize_t *count) {

    int stat = OK;
    user_base_t ondisk;
    ssize_t position = 0;
    off_t offset = sizeof(user_base_t);

    when_error_in {

        stat = files_seek(self->userdb, 0, SEEK_END);
        check_return(stat, self->userdb);

        stat = files_seek(self->userdb, -offset, SEEK_CUR);
        check_return(stat, self->userdb);

        stat = files_tell(self->userdb, &position);
        check_return(stat, self->userdb);

        self->index = USER_RECORD(position);

        stat = self->_lock(self, position);
        check_return(stat, self);

        stat = self->_read(self, &ondisk, count);
        check_return(stat, self);

        stat = self->_unlock(self);
        check_return(stat, self);

        stat = files_seek(self->userdb, -offset, SEEK_CUR);
        check_return(stat, self->userdb);

        if (*count == sizeof(user_base_t)) {

            stat = self->_build(self, &ondisk, user);
            check_return(stat, self);

        }

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

        if (self->locked) self->_unlock(self);

    } end_when;

    return stat;

}

int _user_add(user_t *self, user_base_t *user) {

    int stat = OK;
    ssize_t count = 0;
    user_base_t ondisk;
    int created = FALSE;

    when_error_in {

        stat = self->_first(self, &ondisk, &count);
        check_return(stat, self);

        while (count > 0) {

            if (ondisk.flags & US_INACTIVE) {

                user->eternal = ondisk.eternal;

                stat = self->_put(self, self->index, user);
                check_return(stat, self);

                created = TRUE;
                break;

            }

            stat = self->_next(self, &ondisk, &count);
            check_return(stat, self);

        }

        if (! created) {

            cause_error(EOVERFLOW);

        }

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int _user_del(user_t *self, long userno) {

    int stat = OK;
    ssize_t count = 0;
    user_base_t ondisk;

    when_error_in {

        stat = self->_first(self, &ondisk, &count);
        check_return(stat, self);

        while (count > 0) {

            if ((ondisk.eternal == userno) &&
                (! (ondisk.flags & US_PERM))) {

                ondisk.flags |= US_DELETED;
                ondisk.revision++;

                stat = self->_put(self, self->index, &ondisk);
                check_return(stat, self);

                break;

            }

            stat = self->_next(self, &ondisk, &count);
            check_return(stat, self);

        }

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int _user_get(user_t *self, int index, user_base_t *user) {

    int stat = OK;
    ssize_t count = 0;
    user_base_t ondisk;
    off_t offset = USER_OFFSET(index);

    when_error_in {

        stat = files_seek(self->userdb, offset, SEEK_SET);
        check_return(stat, self->userdb);

        self->_lock(self, offset);
        check_return(stat, self);

        stat = self->_read(self, &ondisk, &count);
        check_return(stat, self);

        stat = self->_unlock(self);
        check_return(stat, self);
        
        if (count == sizeof(user_base_t)) {

            stat = self->_build(self, &ondisk, user);
            check_return(stat, self);

        }

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

        if (self->locked) self->_unlock(self);

    } end_when;

    return stat;

}

int _user_put(user_t *self, int index, user_base_t *user) {

    int stat = OK;
    ssize_t count = 0;
    off_t offset = USER_OFFSET(index);

    when_error_in {

        stat = files_seek(self->userdb, offset, SEEK_SET);
        check_return(stat, self->userdb);

        stat = self->_lock(self, offset);
        check_return(stat, self);

        stat = self->_write(self, user, &count);
        check_return(stat, self);

        stat = self->_unlock(self);
        check_return(stat, self);

        if (count != sizeof(user_base_t)) {

            cause_error(EIO);

        }

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

        if (self->locked) self->_unlock(self);

    } end_when;

    return stat;

}

int _user_lock(user_t *self, off_t offset) {

    int stat = OK;
    int length = sizeof(user_base_t);

    when_error_in {

        stat = files_lock(self->userdb, offset, length);
        check_return(stat, self->userdb);

        self->locked = TRUE;

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int _user_unlock(user_t *self) {

    int stat = OK;

    when_error_in {

        stat = files_unlock(self->userdb);
        check_return(stat, self->userdb);

        self->locked = FALSE;

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int _user_get_sequence(user_t *self, long *sequence) {

    int stat = OK;
    long buffer = 0;
    ssize_t count = 0;
    int locked = FALSE;

    when_error_in {

        stat = files_seek(self->sequence, 0, SEEK_SET);
        check_return(stat, self->sequence);

        stat = files_lock(self->sequence, 0, sizeof(long));
        check_return(stat, self->sequence);

        locked = TRUE;

        stat = files_read(self->sequence, &buffer, sizeof(long), &count);
        check_return(stat, self->sequence);

        if (count != sizeof(long)) {

            cause_error(EIO);

        }

        *sequence = buffer;
        buffer++;

        stat = files_seek(self->sequence, 0, SEEK_SET);
        check_return(stat, self->sequence);

        stat = files_write(self->sequence, &buffer, sizeof(long), &count);
        check_return(stat, self->sequence);

        if (count != sizeof(long)) {

            cause_error(EIO);

        }

        stat = files_unlock(self->sequence);
        check_return(stat, self->sequence);

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

        if (locked) files_unlock(self->sequence);

    } end_when;

    return stat;

}

int _user_extend(user_t *self, int amount) {

    int stat = OK;
    user_base_t user;
    int revision = 1;
    ssize_t count = 0;
    ssize_t position = 0;

    when_error_in {

        memset(&user, '\0', sizeof(user_base_t));

        /* defaults */

        user.revision = revision;
        user.flags |= US_INACTIVE;
        user.qwk = (QWK_FILES | QWK_ATTACH | QWK_EMAIL | QWK_DELMAIL);

        stat = files_seek(self->userdb, 0, SEEK_END);
        check_return(stat, self->userdb);

        int x;
        for (x = 0; x < amount; x++) {

            stat = files_tell(self->userdb, &position);
            check_return(stat, self->userdb);

            stat = self->_lock(self, position);
            check_return(stat, self);

            stat = self->_write(self, &user, &count);
            check_return(stat, self);

            stat = self->_unlock(self);
            check_return(stat, self);

            if (count != sizeof(user_base_t)) {

                cause_error(EIO);

            }

        }

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int _user_build(user_t *self, user_base_t *ondisk, user_base_t *user) {

    strcpy((*user).username, ondisk->username);
    strcpy((*user).password, ondisk->password);
    (*user).axlevel = ondisk->axlevel;
    (*user).qwk = ondisk->qwk;
    (*user).flags = ondisk->flags;
    (*user).screenwidth = ondisk->screenwidth;
    (*user).screenlength = ondisk->screenlength;
    (*user).timescalled = ondisk->timescalled;
    (*user).posted = ondisk->posted;
    (*user).eternal = ondisk->eternal;
    (*user).today = ondisk->today;
    (*user).online = ondisk->online;
    (*user).profile = ondisk->profile;
    (*user).lastcall = ondisk->lastcall;
    (*user).firstcall = ondisk->firstcall;

    return OK;

}

int _user_read(user_t *self, user_base_t *user, ssize_t *count) {

    int stat = OK;

    when_error_in {

        stat = files_read(self->userdb, user, sizeof(user_base_t), count);
        check_return(stat, self->userdb);

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int _user_write(user_t *self, user_base_t *user, ssize_t *count) {

    int stat = OK;

    when_error_in {

        stat = files_write(self->userdb, user, sizeof(user_base_t), count);
        check_return(stat, self->userdb);

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

