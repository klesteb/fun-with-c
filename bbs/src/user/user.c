
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

#include <errno.h>

#include "rms.h"
#include "when.h"
#include "user.h"
#include "files.h"
#include "tracer.h"
#include "item_list.h"

/*----------------------------------------------------------------*/
/* klass overrides                                                */
/*----------------------------------------------------------------*/

int _user_add(rms_t *self, user_base_t *user) {

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

                stat = self->_put(self, self->record, user);
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
 
int _user_build(rms_t *self, user_base_t *ondisk, user_base_t *user) {

    strcpy((*user).username, ondisk->username);
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
    (*user).revision = ondisk->revision;

    return OK;

}
 
int _user_del(rms_t *self, off_t recnum) {

    int stat = OK;
    ssize_t count = 0;
    user_base_t ondisk;
    off_t recsize = self->recsize;
    off_t offset = RMS_OFFSET(recnum, self->recsize);

    when_error_in {

        stat = files_seek(self->rmsdb, offset, SEEK_SET);
        check_return(stat, self->rmsdb);

        stat = self->_lock(self, offset);
        check_return(stat, self);

        stat = self->_read(self, &ondisk, &count);
        check_return(stat, self);

        if (count != self->recsize) {

            cause_error(EIO);

        }

        if (! (ondisk.flags & US_PERM)) {

            ondisk.flags |= US_DELETED;
            ondisk.revision++;

            stat = files_seek(self->rmsdb, -recsize, SEEK_CUR);
            check_return(stat, self->rmsdb);

            stat = self->_write(self, &ondisk, &count);
            check_return(stat, self);

            if (count != self->recsize) {

                cause_error(EIO);

            }

        }

        stat = self->_unlock(self);
        check_return(stat, self);

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

        if (self->locked) self->_unlock(self);

    } end_when;

    return stat;
 
}
 
int _user_extend(rms_t *self, int amount) {

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

        stat = files_seek(self->rmsdb, 0, SEEK_END);
        check_return(stat, self->rmsdb);

        int x;
        for (x = 0; x < amount; x++) {

            stat = files_tell(self->rmsdb, &position);
            check_return(stat, self->rmsdb);

            stat = self->_lock(self, position);
            check_return(stat, self);

            stat = self->_write(self, &user, &count);
            check_return(stat, self);

            stat = self->_unlock(self);
            check_return(stat, self);

            if (count != self->recsize) {

                cause_error(EIO);

            }

        }

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

        if (self->locked) self->_unlock(self);

    } end_when;

    return stat;

}

int _user_init(rms_t *self) {
 
    int stat = OK;
    ssize_t count = 0;
    user_base_t ondisk;
    ssize_t position = 0;

    when_error_in {

        stat = files_seek(self->rmsdb, 0, SEEK_SET);
        check_return(stat, self->rmsdb);

        /* create sysop account */

        ondisk.eternal = 1;
        ondisk.revision = 1;
        ondisk.axlevel = AX_SYSOP;
        ondisk.flags = (US_SYSOP | US_ROOMAIDE | US_PERM | US_SUBSYSTEM | US_NEEDVALID);
        memset(&ondisk.username, '\0', 26);
        strcpy(ondisk.username, "sysop");

        stat = files_tell(self->rmsdb, &position);
        check_return(stat, self->rmsdb);

        stat = self->_lock(self, position);
        check_return(stat, self);

        stat = self->_write(self, &ondisk, &count);
        check_return(stat, self);

        stat = self->_unlock(self);
        check_return(stat, self);

        /* create network account */

        ondisk.eternal = 2;
        ondisk.axlevel = AX_NET;
        ondisk.flags = (US_ROOMAIDE | US_PERM | US_SUBSYSTEM | US_NEEDVALID);
        memset(&ondisk.username, '\0', 26);
        strcpy(ondisk.username, "qwknet");

        stat = files_tell(self->rmsdb, &position);
        check_return(stat, self->rmsdb);

        stat = self->_lock(self, position);
        check_return(stat, self);

        stat = self->_write(self, &ondisk, &count);
        check_return(stat, self);

        stat = self->_unlock(self);
        check_return(stat, self);

        /* guest account */

        ondisk.eternal = 3;
        ondisk.flags = US_PERM;
        ondisk.axlevel = AX_NORM;
        memset(&ondisk.username, '\0', 26);
        strcpy(ondisk.username, "guest");

        stat = files_tell(self->rmsdb, &position);
        check_return(stat, self->rmsdb);

        stat = self->_lock(self, position);
        check_return(stat, self);

        stat = self->_write(self, &ondisk, &count);
        check_return(stat, self);

        stat = self->_unlock(self);
        check_return(stat, self);

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

        if (self->locked) self->_unlock(self);

    } end_when;

    return stat;

}

int _user_normalize(rms_t *self, user_base_t *ondisk, user_base_t *user) {

    strcpy((*user).username, ondisk->username);
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
    (*user).revision = ondisk->revision + 1;

    return OK;

}

int _user_put(rms_t *self, off_t recnum, user_base_t *user) {

    int stat = OK;
    ssize_t count = 0;
    user_base_t ondisk;
    off_t recsize = self->recsize;
    off_t offset = RMS_OFFSET(recnum, self->recsize);

    when_error_in {

        stat = files_seek(self->rmsdb, offset, SEEK_SET);
        check_return(stat, self->rmsdb);

        stat = self->_lock(self, offset);
        check_return(stat, self);

        stat = self->_read(self, &ondisk, &count);
        check_return(stat, self);

        if (ondisk.revision > user->revision) {

            stat = self->_normalize(self, &ondisk, user);
            check_return(stat, self);

        } else {

            user->revision = ondisk.revision + 1;

        }

        stat = files_seek(self->rmsdb, -recsize, SEEK_CUR);
        check_return(stat, self->rmsdb);

        stat = self->_write(self, user, &count);
        check_return(stat, self);

        stat = self->_unlock(self);
        check_return(stat, self);

        if (count != recsize) {

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

/*----------------------------------------------------------------*/
/* klass implementation                                           */
/*----------------------------------------------------------------*/

char *user_version(rms_t *self) {

    char *version = VERSION;

    return version;

}

rms_t *user_create(char *path, int records, int retries, int timeout, tracer_t *dump) {

    int stat = OK;
    rms_t *self = NULL;
    char *name = "users";
    item_list_t items[8];
    int recsize = sizeof(user_base_t);

    when_error_in {

        self = rms_create(path, name, records, recsize, retries, timeout, dump);
        check_creation(self);

        SET_ITEM(items[0], RMS_M_ADD, _user_add, 0, NULL);
        SET_ITEM(items[1], RMS_M_BUILD, _user_build, 0, NULL);
        SET_ITEM(items[2], RMS_M_DEL, _user_del, 0, NULL);
        SET_ITEM(items[3], RMS_M_EXTEND, _user_extend, 0, NULL);
        SET_ITEM(items[4], RMS_M_INIT, _user_init, 0, NULL);
        SET_ITEM(items[5], RMS_M_NORMALIZE, _user_normalize, 0, NULL);
        SET_ITEM(items[6], RMS_M_PUT, _user_put, 0, NULL);
        SET_ITEM(items[7], 0, 0, 0, 0);

        stat = rms_override(self, items);
        check_return(stat, self);

        exit_when;

    } use {

        object_set_error2(self, trace_errnum, trace_lineno, trace_filename, trace_function);
        clear_error();

    } end_when;

    return self;

}

