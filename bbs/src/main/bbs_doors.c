
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
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

#include "bbs/src/main/bbs_common.h"
#include "bbs/src/main/bbs_protos.h"

/* ------------------------------------------------------------------------ */

int _run_door(door_base_t *door, node_base_t *node, 
              room_base_t *room, user_base_t *sysop, 
              user_base_t *user, profile_base_t *profile) {

    int stat = OK;
    char nodenum[8];
    char curdir[256];
    char workdir[256];

    when_error_in {

        /* save current directory */

        memset(curdir, '\0', 255);

        errno = 0;
        if ((getcwd(curdir, 255)) == NULL) {

            cause_error(errno);

        }

        /* change to work directory */

        memset(nodenum, '\0', 8);
        memset(workdir, '\0', 256);

        sprintf(nodenum, "%ld", node->nodenum);
        strncpy(workdir, fnm_build(1, FnmPath, nodenum, WORKPATH, NULL), 255);

        errno = 0;
        stat = chdir(workdir);
        check_status(stat, OK, errno);

        /* write out the drop file */

        if (bit_test(door->flags, DF_DOORSYS)) {

            stat = _write_door_sys(door, node, room, sysop, user, profile);
            check_return(stat, self);

        } 

        if (bit_test(door->flags, DF_DOOR32)) {

            stat = _write_door32_sys(door, node, room, sysop, user, profile);
            check_return(stat, self);

        } 

        if (bit_test(door->flags, DF_DOORINFO)) {

            stat = _write_doorinfo_def(door, node, room, sysop, user, profile);
            check_return(stat, self);

        }

        /* run the door */

        stat = _spawn(door->command);
        check_return(stat, self);

        /* remove the drop file */

        if (bit_test(door->flags, DF_DOORSYS)) {

            stat = _remove_door_sys(door);
            check_return(stat, door);

        } 

        if (bit_test(door->flags, DF_DOOR32)) {

            stat = _remove_door32_sys(door);
            check_return(stat, door);

        } 

        if (bit_test(door->flags, DF_DOORINFO)) {

            stat = _remove_doorinfo_def(door);
            check_return(stat, self);

        }

        /* change back to original directory */

        errno = 0;
        stat = chdir(curdir);
        check_status(stat, OK, errno);

        exit_when;

    } use {

        stat = ERR;
        process_error(self);

    } end_when;

    return stat;

}

static int _remove_door_sys(door_base_t *door) {

    int stat = OK;
    int exists = 0;
    files_t *file = NULL;
    char *filename = "door.sys";

    when_error_in {

        file = files_create(filename, self->retries, self->timeout);
        check_creation(file);

        stat = files_exists(file, &exists);
        check_return(stat, file);

        if (exists) {

            stat = files_unlink(file);
            check_return(stat, file);

        }

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

static int _remove_door32_sys(door_t *self) {

    int stat = OK;
    int exists = 0;
    files_t *file = NULL;
    char *filename = "door32.sys";

    when_error_in {

        file = files_create(filename, self->retries, self->timeout);
        check_creation(file);

        stat = files_exists(file, &exists);
        check_return(stat, file);

        if (exists) {

            stat = files_unlink(file);
            check_return(stat, file);

        }

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

static int _remove_doorinfo_def(door_t *self) {

    int stat = OK;
    int exists = 0;
    files_t *file = NULL;
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

static int _spawn(char *command) {

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
            argv[2] = command;
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

/* ------------------------------------------------------------------------ */

int bbs_doors(rms_t *doors, room_base_t *room, error_trace_t *errors) {

    time_t after;
    int stat = OK;
    time_t before;
    ulong wasted = 0;
    door_base_t door;
    user_base_t sysop;
    error_trace_t error;
    window_t *temp = NULL;
    profile_base_t *profile = NULL;

    when_error_in {

        stat = user_get(users, SYSOP, &sysop);
        check_return(stat, users);

        if (has_profile(&useron)) {

            stat = profile_get(profiles, useron.profile, profile);
            check_return(stat, profile);

        }

        stat = door_get(doors, &door);
        check_return(stat, doors);

        if ((door.active && (! bit_test(door.type, DT_MULTIUSER)))) {

            char *text = "Sorry, this door is already in use. Please check back latter.";

            temp = alert("Sorry", 3, "%s", text);
            check_creation(temp);

            stat = workbench_add(workbench, temp);
            check_return(stat, workbench);

            stat = workbench_set_focus(workbench, temp);
            check_return(stat, workbench);

            stat = workbench_refresh(workbench);
            check_return(stat, workbench);

            exit_when;

        }
        
        door.active = TRUE;
        stat = door_put(doors, &door);
        check_return(stat, doors);

        stat = bbs_send_status(NODE_XTRN, &error);
        check_status2(stat, OK, error);

        before = time(NULL);

        /* exit ncurses */

        def_prog_mode();
        endwin();

        /* run the subsystem */

        stat = _run_door(door, &qnode, room, &sysop, &useron, profile);
        check_return(stat, doors);

        /* reenter ncurses */

        refresh();

        after = time(NULL);
        wasted = ((after - before) / 60);
    
        door.active = FALSE;
        door.usage[useron.eternal - 1].runs++;
        door.usage[useron.eternal - 1].wasted += wasted;
        
        stat = door_put(doors, &door);
        check_return(stat, doors);
            
        exit_when;

    } use {

        stat = ERR;
        copy_error(errors);
        capture_trace(dump);
        clear_error();

    } end_when;

    return stat;

}

