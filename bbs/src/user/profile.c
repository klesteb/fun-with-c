
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

#include "rms/rms.h"
#include "files/files.h"
#include "include/when.h"
#include "tracer/tracer.h"
#include "include/item_list.h"

#include "bbs/src/bitops.h"
#include "bbs/src/user/profile.h"
#include "bbs/src/user/user_common.h"

/*----------------------------------------------------------------*/
/* klass overrides                                                */
/*----------------------------------------------------------------*/

int _profile_add(rms_t *self, profile_base_t *profile) {

    int stat = OK;
    ssize_t count = 0;
    int created = FALSE;
    profile_base_t ondisk;

    when_error_in {

        stat = self->_first(self, &ondisk, &count);
        check_return(stat, self);

        while (count > 0) {

            if (bit_test(ondisk.flags, PF_DELETED)) {

                stat = self->_put(self, self->record, profile);
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
 
int _profile_build(rms_t *self, profile_base_t *ondisk, profile_base_t *profile) {

    (*profile).flags = ondisk->flags;
    strcpy((*profile).name, ondisk->name);
    strcpy((*profile).addr1, ondisk->addr1);
    strcpy((*profile).addr2, ondisk->addr2);
    strcpy((*profile).addr3, ondisk->addr3);
    strcpy((*profile).city, ondisk->city);
    strcpy((*profile).state, ondisk->state);
    strcpy((*profile).zip, ondisk->zip);
    strcpy((*profile).phone, ondisk->phone);
    strcpy((*profile).email, ondisk->email);
    strcpy((*profile).description, ondisk->description);
    (*profile).revision = ondisk->revision;

    return OK;

}
 
int _profile_del(rms_t *self, off_t recnum) {

    int stat = OK;
    ssize_t count = 0;
    profile_base_t ondisk;
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

        bit_set(ondisk.flags, PF_DELETED);
        memset(&ondisk.name, '\0', LEN_NAME+1);
        memset(&ondisk.addr1, '\0', LEN_ADDRESS+1);
        memset(&ondisk.addr2, '\0', LEN_ADDRESS+1);
        memset(&ondisk.addr3, '\0', LEN_ADDRESS+1);
        memset(&ondisk.city, '\0', LEN_CITY+1);
        memset(&ondisk.state, '\0', LEN_STATE+1);
        memset(&ondisk.zip, '\0', LEN_ZIP+1);
        memset(&ondisk.phone, '\0', LEN_PHONE+1);
        memset(&ondisk.email, '\0', LEN_EMAIL+1);
        memset(&ondisk.description, '\0', LEN_DESC+1);
        ondisk.revision++;

        stat = files_seek(self->rmsdb, -recsize, SEEK_CUR);
        check_return(stat, self->rmsdb);

        stat = self->_write(self, &ondisk, &count);
        check_return(stat, self);

        if (count != self->recsize) {

            cause_error(EIO);

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
 
int _profile_extend(rms_t *self, int amount) {

    int stat = OK;
    ssize_t count = 0;
    ssize_t position = 0;
    profile_base_t profile;

    when_error_in {

        memset(&profile, '\0', sizeof(profile_base_t));
        profile.flags |= PF_DELETED;
        profile.revision = 1;

        stat = files_seek(self->rmsdb, 0, SEEK_END);
        check_return(stat, self->rmsdb);

        int x = 0;
        for (; x < amount; x++) {

            stat = files_tell(self->rmsdb, &position);
            check_return(stat, self->rmsdb);

            stat = self->_lock(self, position);
            check_return(stat, self);

            stat = self->_write(self, &profile, &count);
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

int _profile_normalize(rms_t *self, profile_base_t *ondisk, profile_base_t *profile) {

    (*profile).flags = ondisk->flags;
    strcpy((*profile).name, ondisk->name);
    strcpy((*profile).addr1, ondisk->addr1);
    strcpy((*profile).addr2, ondisk->addr2);
    strcpy((*profile).addr3, ondisk->addr3);
    strcpy((*profile).city, ondisk->city);
    strcpy((*profile).state, ondisk->state);
    strcpy((*profile).zip, ondisk->zip);
    strcpy((*profile).phone, ondisk->phone);
    strcpy((*profile).email, ondisk->email);
    strcpy((*profile).description, ondisk->description);
    (*profile).revision = ondisk->revision + 1;

    return OK;

}

int _profile_put(rms_t *self, off_t recnum, profile_base_t *profile) {

    int stat = OK;
    ssize_t count = 0;
    profile_base_t ondisk;
    off_t recsize = self->recsize;
    off_t offset = RMS_OFFSET(recnum, self->recsize);

    when_error_in {

        stat = files_seek(self->rmsdb, offset, SEEK_SET);
        check_return(stat, self->rmsdb);

        stat = self->_lock(self, offset);
        check_return(stat, self);

        stat = self->_read(self, &ondisk, &count);
        check_return(stat, self);

        if (ondisk.revision > profile->revision) {

            stat = self->_normalize(self, &ondisk, profile);
            check_return(stat, self);

        } else {

            profile->revision = ondisk.revision + 1;

        }

        stat = files_seek(self->rmsdb, -recsize, SEEK_CUR);
        check_return(stat, self->rmsdb);

        stat = self->_write(self, profile, &count);
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

int profile_capture(rms_t *self, void *data, queue *results) {

    int stat = OK;
    profile_base_t *ondisk = NULL;
    profile_search_t *result = NULL;

    when_error_in {

        ondisk = (profile_base_t *)data;

        errno = 0;
        result = calloc(1, sizeof(profile_search_t));
        if (result == NULL) cause_error(errno);

        strncpy(result->name, ondisk->name, LEN_NAME);
        result->record = self->record;

        stat = que_push_head(results, result);
        check_status(stat, QUE_OK, E_NOQUEUE);

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;
    
    return stat;

}

char *profile_version(rms_t *self) {

    char *version = VERSION;

    return version;

}

rms_t *profile_create(char *path, int records, int retries, int timeout, tracer_t *dump) {

    int stat = OK;
    rms_t *self = NULL;
    item_list_t items[7];
    char *name = "profiles";
    int recsize = sizeof(profile_base_t);

    when_error_in {

        self = rms_create(path, name, records, recsize, retries, timeout, dump);
        check_creation(self);

        SET_ITEM(items[0], RMS_M_ADD, _profile_add, 0, NULL);
        SET_ITEM(items[1], RMS_M_BUILD, _profile_build, 0, NULL);
        SET_ITEM(items[2], RMS_M_DEL, _profile_del, 0, NULL);
        SET_ITEM(items[3], RMS_M_EXTEND, _profile_extend, 0, NULL);
        SET_ITEM(items[4], RMS_M_NORMALIZE, _profile_normalize, 0, NULL);
        SET_ITEM(items[5], RMS_M_PUT, _profile_put, 0, NULL);
        SET_ITEM(items[6], 0, 0, 0, 0);

        stat = rms_override(self, items);
        check_return(stat, self);

        exit_when;

    } use {

        process_error(self);

    } end_when;

    return self;

}

