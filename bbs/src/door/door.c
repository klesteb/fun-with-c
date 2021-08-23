
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

#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

#include "include/when.h"
#include "include/error_codes.h"

#include "gpl/fnm_util.h"
#include "objects/object.h"
#include "cclibs/misc/misc.h"

#include "bbs/src/bitops.h"
#include "bbs/src/door/door.h"
#include "bbs/src/main/bbs_config.h"
#include "bbs/src/main/bbs_protos.h"
#include "bbs/src/main/bbs_error_codes.h"

require_klass(OBJECT_KLASS);

/*----------------------------------------------------------------*/
/* klass methods                                                  */
/*----------------------------------------------------------------*/

int _door_ctor(object_t *, item_list_t *);
int _door_dtor(object_t *);

int _door_compare(door_t *, door_t *);
int _door_override(door_t *, item_list_t *);
int _door_open(door_t *);
int _door_close(door_t *);
int _door_remove(door_t *);
int _door_run(door_t *, node_base_t *, room_base_t *, user_base_t *, user_base_t *, profile_base_t *) ;

/*----------------------------------------------------------------*/
/* klass private methods                                          */
/*----------------------------------------------------------------*/

static int _spawn(door_t *);
static int _write_door_sys(door_t *, node_base_t *, room_base_t *, user_base_t *, user_base_t *, profile_base_t *);
static int _write_door32_sys(door_t *, node_base_t *, room_base_t *, user_base_t *, user_base_t *, profile_base_t *);
static int _write_doorinfo_def(door_t *, node_base_t *, room_base_t *, user_base_t *, user_base_t *, profile_base_t *);

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

door_t *door_create(char *path, char *name, int retries, int timeout, tracer_t *dump) {

    int stat = ERR;
    door_t *self = NULL;
    item_list_t items[5];

    SET_ITEM(items[0], DOOR_K_PATH, path, strlen(path), NULL);
    SET_ITEM(items[1], DOOR_K_NAME, name, strlen(name), NULL);
    SET_ITEM(items[2], DOOR_K_RETRIES, &retries, sizeof(int), NULL);
    SET_ITEM(items[3], DOOR_K_TIMEOUT, &timeout, sizeof(int), NULL);
    SET_ITEM(items[4], DOOR_K_TRACE, dump, 0, NULL);
    SET_ITEM(items[5], 0, 0, 0, 0);

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

int door_open(door_t *self) {

    int stat = OK;

    when_error {

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

int door_close(door_t *self) {

    int stat = OK;

    when_error {

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

int door_remove(door_t *self) {

    int stat = OK;

    when_error {

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

int door_run(door_t *self, node_base_t *node, room_base_t *room, user_base_t *sysop, user_base_t *user, profile_base_t *profile) {

    int stat = OK;

    when_error {

        if ((self == NULL) || (room == NULL) || (node == NULL) ||
            (sysop == NULL) || (user == NULL)) {

            cause_error(E_INVPARM);

        }

        stat = self->_run(self, node, room, sysop, user, profile);
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

int _door_ctor(object_t *object, item_list_t *items) {

    char name[32];
    char path[256];
    int stat = ERR;
    int timeout = 1;
    int retries = 30;
    door_t *self = NULL;
    tracer_t *dump = NULL;

    if (object != NULL) {

        memset(name, '\0', 32);
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
                    case DOOR_K_NAME: {
                        memcpy(name, 
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
        self->_open     = _door_open;
        self->_close    = _door_close;
        self->_remove   = _door_remove;
        self->_run      = _door_run;

        /* initialize internal variables here */

        when_error_in {

            self->flags = 0;
            self->trace = dump;
            self->retries = retries;
            self->timeout = timeout;

            strncpy(path, fnm_build(1, FnmPath, name, ".ctl", path, NULL), 255);
            self->control = files_create(path, retries, timeout);
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

    free(self->command);

    files_close(self->control);
    files_destroy(self->control);

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
                case DOOR_M_REMOVE: {
                    self->dtor = items[x].buffer_address;
                    stat = OK;
                    break;
                }
                case DOOR_M_RUN: {
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
        (this->_override == that->_override) &&
        (this->_open == that->_open) &&
        (this->_close == that->_close) &&
        (this->_remove == that->_remove) &&
        (this->_run == that->_run)) {

        stat = OK;

    }

    return stat;

}

int _door_open(door_t *self) {

    int stat = OK;
    int exists = 0;
    char buffer[1024];
    ssize_t count = 0;
    int flags = O_RDWR;
    char *holder = NULL;

    when_error_in {

        stat = files_exists(self->control, &exists);
        check_return(stat, self->control);

        if (exists) {

            stat = files_open(self->control, flags, 0);
            check_return(stat, self->control);

            /* command line */

            memset(buffer, '\0', 1024);
            stat = files_gets(self->control, buffer, 1023, &count);
            check_return(stat, self->control);
            if (count < 1) cause_error(EIO);

            self->command = strndup(buffer, 255);

            /* flags: a comma seperated field  */

            memset(buffer, '\0', 1024);
            stat = files_gets(self->control, buffer, 1023, &count);
            check_return(stat, self->control);
            if (count < 1) cause_error(EIO);

            /* flag values: door32, doorsys, doorinfo, none */

            for (holder = xstrtok(buffer, ",");
                 holder != NULL;
                 holder = xstrtok(NULL, ",")) {

                if (stricmp("DOORSYS", holder) == 0) {

                    bit_set(self->flags, DF_DOORSYS);

                } else if (stricmp("DOOR32", holder) == 0) {

                    bit_set(self->flags, DF_DOOR32);

                } else if (stricmp("DOORINFO", holder) == 0) {

                    bit_set(self->flags, DF_DOORINFO);

                }

            }

        } else {

            cause_error(E_UNKFILE);

        }

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int _door_close(door_t *self) {

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

int _door_remove(door_t *self) {

    int stat = OK;

    when_error_in {

        stat = files_close(self->control);
        check_return(stat, self->control);

        stat = files_unlink(self->control);
        check_return(stat, self->control);

        stat = files_destroy(self->control);
        check_return(stat, self->control);

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

int _door_run(door_t *self, node_base_t *node, 
              room_base_t *room, user_base_t *sysop, 
              user_base_t *user, profile_base_t *profile) {

    int stat = OK;

    when_error_in {

        if (bit_test(self->flags, DF_DOORSYS)) {

            stat = _write_door_sys(self, node, room, sysop, user, profile);
            check_return(stat, self);

        } 

        if (bit_test(self->flags, DF_DOOR32)) {

            stat = _write_door32_sys(self, node, room, sysop, user, profile);
            check_return(stat, self);

        } 

        if (bit_test(self->flags, DF_DOORINFO)) {

            stat = _write_doorinfo_def(self, node, room, sysop, user, profile);
            check_return(stat, self);

        }

        stat = _spawn(self);
        check_return(stat, self);

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

static int _write_doorinfo_def(
    door_t *self, 
    node_base_t *node, 
    room_base_t *room, 
    user_base_t *sysop, 
    user_base_t *user, 
    profile_base_t *profile) {

    int stat = OK;
    char temp2[37];
    int exists = 0;
    char buffer[256];
    char *temp3 = NULL;
    ssize_t count = 0;
    files_t *file = NULL;
    int mode = (S_IRWXU | S_IRWXG);
    int create = (O_RDWR | O_CREAT);
    char *filename = "doorinfo1.def";

    when_error_in {

        file = files_create(filename, self->retries, self->timeout);
        check_creation(file);

        stat = files_exists(file, &exists);
        check_return(stat, file);

        if (exists) {

            stat = files_unlink(file);
            check_return(stat, file);

        }

        stat = files_open(file, create, mode);
        check_return(stat, file);

        stat = files_set_eol(file, "\r\n");
        check_return(stat, file);

        /* line 1 - system name */

        stat = files_puts(file, HUMANNODE, &count);
        check_return(stat, file);
        if (count < 0) cause_error(EIO);

        /* line 2,3 - sysop's name; first, last*/

        memset(temp2, '\0', 37);
        snprintf(temp2, 36, "%s", sysop->username);
        strtok(temp2, " ");
        temp3 = strtok(NULL, " ");

        if (temp3 == NULL) {

            stat = files_puts(file, temp2, &count);
            check_return(stat, file);

            stat = files_puts(file, "", &count);
            check_return(stat, file);

        } else {

            stat = files_puts(file, temp2, &count);
            check_return(stat, file);

            stat = files_puts(file, temp3, &count);
            check_return(stat, file);

        }

        /* line 4 - com port, always COM0 */

        memset(buffer, '\0', 256);
        strcpy(buffer, "COM0");
        stat = files_puts(file, buffer, &count);
        check_return(stat, file);

        /* line 5 - connection speed */

        memset(buffer, '\0', 256);
        strcpy(buffer, "19200 BAUD,N,8,1");
        stat = files_puts(file, buffer, &count);
        check_return(stat, file);

        /* line 6 - networked ?? */

        memset(buffer, '\0', 256);
        strcpy(buffer, "0");
        stat = files_puts(file, buffer, &count);
        check_return(stat, file);

        /* lines 7,8 - users name */

        memset(temp2, '\0', 37);
        snprintf(temp2, 36, "%s", user->username);
        strtok(temp2, " ");
        temp3 = strtok(NULL, " ");

        if (temp3 == NULL) {

            stat = files_puts(file, temp2, &count);
            check_return(stat, file);

            stat = files_puts(file, "", &count);
            check_return(stat, file);

        } else {

            stat = files_puts(file, temp2, &count);
            check_return(stat, file);

            stat = files_puts(file, temp3, &count);
            check_return(stat, file);

        }

        /* line 9 - location */

        memset(buffer, '\0', 256);

        if (profile != NULL) {

            snprintf(buffer, 31, "%s, %s", profile->city, profile->state);
            stat = files_puts(file, buffer, &count);
            check_return(stat, file);

        } else {

            strcpy(buffer, "unknown");

            stat = files_puts(file, buffer, &count);
            check_return(stat, file);

        }

        /* line 10 - emulation "0" if TTY, or "1" if ANSI. */

        memset(buffer, '\0', 256);
        strcpy(buffer, "1");
        stat = files_puts(file, buffer, &count);
        check_return(stat, file);

        /* line 11 - security level */

        memset(buffer, '\0', 256);

        if (is_norm(room, user)) {

            strcpy(buffer, "30");

        } else if (is_aide(room, user)) {

            strcpy(buffer, "80");

        } else if (is_sysop(room, user)) {

            strcpy(buffer, "100");

        } else {

            strcpy(buffer, "5");

        }

        stat = files_puts(file, buffer, &count);
        check_return(stat, file);

        /* line 12 - time left */

        memset(buffer, '\0', 256);
        snprintf(buffer, 7, "%ld", (user->timelimit - TIMELIM));

        stat = files_puts(file, buffer, &count);
        check_return(stat, file);

        /* line 13 - FOSSIL driver "-1" if external, "0" if internal */

        memset(buffer, '\0', 256);
        strcpy(buffer, "-1");
        stat = files_puts(file, buffer, &count);
        check_return(stat, file);

        /* all done */

        stat = files_close(file);
        check_return(stat, file);

        stat = files_destroy(file);
        check_return(stat, file);

        exit_when;

    } use {

        stat = ERR;
        capture_trace(self->trace);
        clear_error();

    } end_when;

    return stat;

}

static int _write_door32_sys(
    door_t *self, 
    node_base_t *node, 
    room_base_t *room, 
    user_base_t *sysop, 
    user_base_t *user, 
    profile_base_t *profile) {

    int stat = OK;
    int exists = 0;
    char buffer[256];
    ssize_t count = 0;
    files_t *file = NULL;
    char *filename = "door32.sys";
    int mode = (S_IRWXU | S_IRWXG);
    int create = (O_RDWR | O_CREAT);

    when_error_in {

        file = files_create(filename, self->retries, self->timeout);
        check_creation(file);

        stat = files_exists(file, &exists);
        check_return(stat, file);

        if (exists) {

            stat = files_unlink(file);
            check_return(stat, file);

        }

        stat = files_open(file, create, mode);
        check_return(stat, file);

        stat = files_set_eol(file, "\r\n");
        check_return(stat, file);

        /* line 1 - com port, 0=local, 1=serial, 2=telnet */

        strcpy(buffer, "0");
        stat = files_puts(file, buffer, &count);
        check_return(stat, file);

        /* line 2 - com port, number or socket */

        memset(buffer, '\0', 256);
        strcpy(buffer, "0");
        stat = files_puts(file, buffer, &count);
        check_return(stat, file);

        /* line 3 - effective baud rate */

        memset(buffer, '\0', 256);
        strcpy(buffer, "0");
        stat = files_puts(file, buffer, &count);
        check_return(stat, file);

        /* line 4 - bbs name */

        stat = files_puts(file, HUMANNODE, &count);
        check_return(stat, file);

        /* line 5 - user record */

        memset(buffer, '\0', 256);
        snprintf(buffer, 7, "%ld", user->eternal);

        stat = files_puts(file, buffer, &count);
        check_return(stat, file);

        /* line 6 - user name */

        if (profile != NULL) {

            stat = files_puts(file, profile->name, &count);
            check_return(stat, file);

        } else {

            stat = files_puts(file, user->username, &count);
            check_return(stat, file);

        }


        /* line 7 - user handle */

        stat = files_puts(file, user->username, &count);
        check_return(stat, file);

        /* line 8 - security level */

        memset(buffer, '\0', 256);

        if (is_norm(room, user)) {

            strcpy(buffer, "30");

        } else if (is_aide(room, user)) {

            strcpy(buffer, "80");

        } else if (is_sysop(room, user)) {

            strcpy(buffer, "100");

        } else {

            strcpy(buffer, "5");

        }

        stat = files_puts(file, buffer, &count);
        check_return(stat, file);

        /* line 9 - time left */

        memset(buffer, '\0', 256);
        snprintf(buffer, 7, "%ld", (user->timelimit - TIMELIM));

        stat = files_puts(file, buffer, &count);
        check_return(stat, file);

        /* line 10 - graphics mode */
        /* 0 = Ascii, 1 = Ansi, 2 = Avatar, 3 = RIP, 4 = Max Graphics */

        memset(buffer, '\0', 256);
        strcpy(buffer, "1");
        stat = files_puts(file, buffer, &count);
        check_return(stat, file);

        /* line 11 - node number */

        memset(buffer, '\0', 256);
        snprintf(buffer, 3, "%ld", node->nodenum);

        stat = files_puts(file, buffer, &count);
        check_return(stat, file);

        /* all done */

        stat = files_close(file);
        check_return(stat, file);

        stat = files_destroy(file);
        check_return(stat, file);

        exit_when;

    } use {

        stat = ERR;
        capture_trace(self->trace);
        clear_error();

    } end_when;

    return stat;

}

static int _write_door_sys(
    door_t *self, 
    node_base_t *node, 
    room_base_t *room, 
    user_base_t *sysop, 
    user_base_t *user, 
    profile_base_t *profile) {

    int stat = OK;
    int exists = 0;
    char buffer[256];
    ssize_t count = 0;
    files_t *file = NULL;
    char *filename = "door.sys";
    int mode = (S_IRWXU | S_IRWXG);
    int create = (O_RDWR | O_CREAT);

    when_error_in {

        file = files_create(filename, self->retries, self->timeout);
        check_creation(file);

        stat = files_exists(file, &exists);
        check_return(stat, file);

        if (exists) {

            stat = files_unlink(file);
            check_return(stat, file);

        }

        stat = files_open(file, create, mode);
        check_return(stat, file);

        stat = files_set_eol(file, "\r\n");
        check_return(stat, file);

        /* line 1 - com port */

        memset(buffer, '\0', 256);
        strcpy(buffer, "COM0:");
        stat = files_puts(file, buffer, &count);
        check_return(stat, file);

        /* line 2 - efective baud rate */

        memset(buffer, '\0', 256);
        strcpy(buffer, "0");
        stat = files_puts(file, buffer, &count);
        check_return(stat, file);

        /* line 3 - data bits */

        memset(buffer, '\0', 256);
        strcpy(buffer, "8");
        stat = files_puts(file, buffer, &count);
        check_return(stat, file);

        /*line 4 - node number */

        memset(buffer, '\0', 256);
        snprintf(buffer, 3, "%ld", node->nodenum);

        stat = files_puts(file, buffer, &count);
        check_return(stat, file);

        /* line 5 - locked baud rate */

        memset(buffer, '\0', 256);
        strcpy(buffer, "19200");
        stat = files_puts(file, buffer, &count);
        check_return(stat, file);

        /* line 6 - screen snoop */

        stat = files_puts(file, "Y", &count);
        check_return(stat, file);

        /* line 7 - printer on */

        stat = files_puts(file, "N", &count);
        check_return(stat, file);

        /* line 8 - pager bell */

        stat = files_puts(file, "Y", &count);
        check_return(stat, file);

        /* line 9 - caller alarm */

        stat = files_puts(file, "Y", &count);
        check_return(stat, file);

        /* line 10, username */

        stat = files_puts(file, user->username, &count);
        check_return(stat, file);

        /* line 11, location */

        memset(buffer, '\0', 256);

        if (profile != NULL) {

            snprintf(buffer, 31, "%s, %s", profile->city, profile->state);
            stat = files_puts(file, buffer, &count);
            check_return(stat, file);

        } else {

            strcpy(buffer, "unknown");

            stat = files_puts(file, buffer, &count);
            check_return(stat, file);

        }

        /* line 12, voice phone */

        if (profile != NULL) {

            stat = files_puts(file, profile->phone, &count);
            check_return(stat, file);

        } else {

            memset(buffer, '\0', 256);
            strcpy(buffer, "(216) 867-5309");

            stat = files_puts(file, buffer, &count);
            check_return(stat, file);

        }

        /* line 13, data phone */

        memset(buffer, '\0', 256);
        strcpy(buffer, "(216) 606-0842");

        stat = files_puts(file, buffer, &count);
        check_return(stat, file);

        /* line 14, password */

        stat = files_puts(file, "", &count);
        check_return(stat, file);

        /* line 15, security level */

        memset(buffer, '\0', 256);

        if (is_norm(room, user)) {

            strcpy(buffer, "30");

        } else if (is_aide(room, user)) {

            strcpy(buffer, "80");

        } else if (is_sysop(room, user)) {

            strcpy(buffer, "100");

        } else {

            strcpy(buffer, "5");

        }

        stat = files_puts(file, buffer, &count);
        check_return(stat, file);

        /* line 16 - total calls */

        memset(buffer, '\0', 256);
        sprintf(buffer, "%d", user->timescalled);

        stat = files_puts(file, buffer, &count);
        check_return(stat, file);

        /* line 17, last login date */

        memset(buffer, '\0', 256);
        sprintf(buffer, "%s", gdate(user->lastcall, TRUE));

        stat = files_puts(file, buffer, &count);
        check_return(stat, file);

        /* line 18, time left in seconds */

        memset(buffer, '\0', 256);
        snprintf(buffer, 7, "%ld", ((user->timelimit - TIMELIM) * 60));

        stat = files_puts(file, buffer, &count);
        check_return(stat, file);

        /* line 19, time left in minutes */

        memset(buffer, '\0', 256);
        snprintf(buffer, 7, "%ld", (user->timelimit - TIMELIM));

        stat = files_puts(file, buffer, &count);
        check_return(stat, file);

        /* line 20, graphics GR, RIP, NG */

        memset(buffer, '\0', 256);
        strcpy(buffer, "GR");

        stat = files_puts(file, buffer, &count);
        check_return(stat, file);

        /* line 21, number of screen rows */

        memset(buffer, '\0', 256);
        snprintf(buffer, 3, "%d", user->rows);

        stat = files_puts(file, buffer, &count);
        check_return(stat, file);

        /* line 22, user mode, always N */

        stat = files_puts(file, "N", &count);
        check_return(stat, file);

        /* line 23, always blank */

        stat = files_puts(file, "", &count);
        check_return(stat, file);

        /* line 24, always blank */

        stat = files_puts(file, "", &count);
        check_return(stat, file);

        /* line 25, expiration date */

        memset(buffer, '\0', 256);
        strcpy(buffer, "12-31-1969");

        stat = files_puts(file, buffer, &count);
        check_return(stat, file);

        /* line 26, user number */

        memset(buffer, '\0', 256);
        sprintf(buffer, "%ld", user->eternal);

        stat = files_puts(file, buffer, &count);
        check_return(stat, file);

        /* line 27, transfer protocol */

        stat = files_puts(file, "", &count);
        check_return(stat, file);

        /* line 28, uploads */

        stat = files_puts(file, "0", &count);
        check_return(stat, file);

        /* line 29, downloads */

        stat = files_puts(file, "0", &count);
        check_return(stat, file);

        /* line 30, upload kbs today */

        stat = files_puts(file, "0", &count);
        check_return(stat, file);

        /* line 31, download kbs today */

        stat = files_puts(file, "0", &count);
        check_return(stat, file);

        /* line 32, date of birth */

        memset(buffer, '\0', 256);
        strcpy(buffer, "12-31-1969");

        stat = files_puts(file, buffer, &count);
        check_return(stat, file);

        /* line 33, path to userbase */

        stat = files_puts(file, "", &count);
        check_return(stat, file);

        /* line 34, path to message base */

        stat = files_puts(file, "", &count);
        check_return(stat, file);

        /* line 35, sysop name */

        stat = files_puts(file, sysop->username, &count);
        check_return(stat, file);

        /* line 36, user handle */

        stat = files_puts(file, user->username, &count);
        check_return(stat, file);

        /* line 37, next event time */

        memset(buffer, '\0', 256);
        strcpy(buffer, "none");

        stat = files_puts(file, buffer, &count);
        check_return(stat, file);

        /* line 38, error free connect */

        stat = files_puts(file, "Y", &count);
        check_return(stat, file);

        /* line 39, always N */

        stat = files_puts(file, "N", &count);
        check_return(stat, file);

        /* line 40, always Y */

        stat = files_puts(file, "Y", &count);
        check_return(stat, file);

        /* line 41, default text colors */

        stat = files_puts(file, "7", &count);
        check_return(stat, file);

        /* line 42, always 0 */

        stat = files_puts(file, "0", &count);
        check_return(stat, file);

        /* line 43, last new files scan date */

        memset(buffer, '\0', 256);
        sprintf(buffer, "%s", gdate(user->lastcall, TRUE));

        stat = files_puts(file, buffer, &count);
        check_return(stat, file);

        /* line 44, time of this call */

        memset(buffer, '\0', 256);
        sprintf(buffer, "%s", str_time_hm(user->lastcall));

        stat = files_puts(file, buffer, &count);
        check_return(stat, file);

        /* line 45, time of last call */

        memset(buffer, '\0', 256);
        sprintf(buffer, "%s", gdate(user->lastcall, TRUE));

        stat = files_puts(file, buffer, &count);
        check_return(stat, file);

        /* line 46, always 32768 */

        memset(buffer, '\0', 256);
        strcpy(buffer, "32768");

        stat = files_puts(file, buffer, &count);
        check_return(stat, file);

        /* line 47, downloads today */

        stat = files_puts(file, "0", &count);
        check_return(stat, file);

        /* line 48, uploads today */

        stat = files_puts(file, "0", &count);
        check_return(stat, file);

        /* line 49, downloads to day */

        stat = files_puts(file, "0", &count);
        check_return(stat, file);

        /* line 50, comment */

        stat = files_puts(file, "", &count);
        check_return(stat, file);

        /* line 51, always 0 */

        stat = files_puts(file, "0", &count);
        check_return(stat, file);

        /* line 52, number of messages posted */

        stat = files_puts(file, "0", &count);
        check_return(stat, file);

        /* all done */

        stat = files_close(file);
        check_return(stat, file);

        stat = files_destroy(file);
        check_return(stat, file);

        exit_when;

    } use {

        stat = ERR;
        capture_trace(self->trace);
        clear_error();

    } end_when;

    return stat;

}

static int _spawn(door_t *self) {

    int	rc = 0;
    pid_t pid = 0;
    int stat = OK;
    char *argv[4];
    int status = 0;
    extern char **environ;
    int opts = (WUNTRACED | WCONTINUED);

    when_error_in {

        pid = fork();

        if ((pid == 0)) {

            /* child process */

            argv[0] = (char *)"sh";
            argv[1] = (char *)"-c";
            argv[2] = self->command;
            argv[3] = 0;
            execve("/bin/sh", argv, environ);
            exit(EXIT_FAILURE);

        } else {

            /* parent process, waiting for the child */

            do {

                errno = 0;
                rc = waitpid(pid, &status, opts);
                if ((rc < 1)) cause_error(errno);

                if (WIFEXITED(status)) {

                    if (WEXITSTATUS(status) > 0) {

                        cause_error(E_DOOREXIT);

                    }

                }

            } while (!WIFEXITED(status) && !WIFSIGNALED(status));

        }

        exit_when;

    } use {

        stat = ERR;
        capture_trace(self->trace);
        clear_error();

    } end_when;

	return stat;

}

