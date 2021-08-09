
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

#include "bbs/src/main/bbs_common.h"
#include "bbs/src/main/bbs_protos.h"

/* local items ---------------------------------------------------------- */

static char *str_time_hm(time_t date) {

	static char	ttime[9];
	struct tm	*l_d;

	l_d = localtime(&date);
	snprintf(ttime, 9, "%02d:%02d", l_d->tm_hour, l_d->tm_min);

	return ttime;

}

static char *str_time_hms(time_t date) {

	static char	ttime[9];
	struct tm	*l_d;

	l_d = localtime(&date);
	snprintf(ttime, 9, "%02d:%02d:%02d", l_d->tm_hour, l_d->tm_min, l_d->tm_sec);

	return ttime;

}

static char *gdate(time_t tt, int Y2K) {

    static char glc[15];
    struct tm   *tm;

    tm = localtime(&tt);
    if (Y2K) {

        snprintf(glc, 15, "%02d-%02d-%04d", tm->tm_mon + 1, tm->tm_mday, tm->tm_year + 1900);

    } else {

        snprintf(glc, 15, "%02d-%02d-%02d", tm->tm_mon + 1, tm->tm_mday, tm->tm_year % 100);

    }

    return glc;

}

static int write_doorinfo_def(room_base_t *room, nodes_base_t *node, user_base_t *user) {

    int stat = OK;
    char temp2[37];
    int exists = 0;
    char buffer[256];
    user_base_t sysop;
    char filename[256];
    char *temp3 = NULL;
    files_t *file = NULL;
    int sysopnum = SYSOP;
    profile_base_t profile;
    int mode = (S_IRWXU | S_IRWXG);
    int create = (O_RDWR | O_CREAT);

    when_error_in {

        stat = user_get(users, sysopnum, &sysop);
        check_return(stat, users);

        memset(filename, '\0', 256);
        strncpy(filename, fnm_build(1, FnmPath, "doorinfo1", ".def", node->nodenum, workpath, NULL), 255);

        file = files_create(filename, retries, timeout);
        check_creation(file);

        stat = files_exists(file, &exists);
        check_return(stat, file);

        if (exists) {

            stat = files_unlink(file);
            check_return(stat, file);

        }

        stat = files_open(file, create, mode);
        check_return(stat, file);

        stat = files_set_eol(file, "\012\013");
        check_return(stat, file);

        /* line 1 - system name */

        stat = files_puts(file, humannode, strlen(humannode));
        check_return(stat, file);

        /* line 2,3 - sysop's name; first, last*/

        memcpy(temp2, '\0', 37);
        snprintf(temp2, 36, "%s", sysop.name);
        strtok(temp2, " ");
        temp3 = strtok(NULL, " ");

        if (temp3 == NULL) {

            stat = files_puts(file, temp2, strlen(temp2));
            check_return(stat, file);

            stat = files_puts(file, "", 1);
            check_return(stat, file);

        } else {

            stat = files_puts(file, temp2, strlen(temp2));
            check_return(stat, file);

            stat = files_puts(file, temp3, strlen(temp3));
            check_return(stat, file);

        }

        /* line 4 - com port, always COM0 */

        memset(buffer, '\0', 256);
        strcpy(buffer, "COM0");
        stat = files_puts(file, buffer, strlen(buffer));
        check_return(stat, file);

        /* line 5 - connection speed */

        memset(buffer, '\0', 256)
        strcpy(buffer, "19200 BAUD,N,8,1");
        stat = files_puts(file, buffer, strlen(buffer));
        check_return(stat, file);

        /* line 6 - networked ?? */

        memset(buffer, '\0', 256);
        strcpy(buffer, "0");
        stat = files_puts(file, buffer, strlen(buffer));
        check_return(stat, file);

        /* lines 7,8 - users name */

        memset(temp2, '\0', 37);
        snprintf(temp2, 36, "%s", user->name);
        strtok(temp2, " ");
        temp3 = strtok(NULL, " ");

        if (temp3 == NULL) {

            stat = files_puts(file, temp2, strlen(temp2));
            check_return(stat, file);

            stat = files_puts(file, "", 1);
            check_return(stat, file);

        } else {

            stat = files_puts(file, temp2, strlen(temp2));
            check_return(stat, file);

            stat = files_puts(file, temp3, strlen(temp3));
            check_return(stat, file);

        }

        /* line 9 - location */

        memset(buffer, '\0', 256);

        if (has_profile(user)) {

            stat = profile_get(profiles, user->profile, &profile);
            check_return(stat, profiles);

            snprintf(buffer, 31, "%s, %s", profile.city, profile.state);
            stat = files_puts(file, buffer, strlen(buffer));
            check_return(stat, file);

        } else {

            strcpy(buffer, "unknown");

            stat = files_puts(file, buffer, strlen(buffer));
            check_return(stat, file);

        }

        /* line 10 - emulation "0" if TTY, or "1" if ANSI. */

        memset(buffer, '\0', 256);
        strcpy(buffer, "1");
        stat = files_puts(file, buffer, strlen(buffer));
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

        stat = files_puts(file, buffer, strlen(buffer));
        check_return(stat, file);

        /* line 12 - time left */

        memset(buffer, '\0', 256);
        snprintf(buffer, 7, "%d", (user->timelimit - timelim));

        stat = files_puts(file, buffer, strlen(buffer));
        check_return(stat, file);

        /* line 13 - FOSSIL driver "-1" if external, "0" if internal */

        memset(buffer, '\0', 256);
        strcpy(bufger, "-1");
        stat = files_puts(file, buffer, strlen(buffer));
        check_return(stat, file);

        /* all done */

        stat = files_close(file);
        check_return(stat, file);

        stat = files_destroy(file);
        check_return(stat, file);

        exit_when;

    } use {

        stat = ERR;
        copy_error(errors);
        capture_trace(dump);
        clear_error();

    } end_when;

    return stat;

}

static int write_door32_sys(room_base_t *room, nodes_base_t *node, user_base_t *user) {

    int stat = OK;
    int exists = 0;
    char buffer[256];
    user_base_t sysop;
    char filename[256];
    files_t *file = NULL;
    int sysopnum = SYSOP;
    profile_base_t profile;
    int mode = (S_IRWXU | S_IRWXG);
    int create = (O_RDWR | O_CREAT);

    when_error_in {

        stat = user_get(users, sysopnum, &sysop);
        check_return(stat, users);

        memset(buffer, '\0', 256);
        memset(filename, '\0', 256);
        strncpy(filename, fnm_build(1, FnmPath, "door32", ".sys", node->nodenum, workpath, NULL), 255);

        file = files_create(filename, retries, timeout);
        check_creation(file);

        stat = files_exists(file, &exists);
        check_return(stat, file);

        if (exists) {

            stat = files_unlink(file);
            check_return(stat, file);

        }

        stat = files_open(file, create, mode);
        check_return(stat, file);

        stat = files_set_eol(file, "\012\013");
        check_return(stat, file);

        /* line 1 - com port, 0=local, 1=serial, 2=telnet */

        strcpy(buffer, "0");
        stat = files_puts(file, buffer, strlen(buffer));
        check_return(stat, file);

        /* line 2 - com port, number or socket */

        memset(buffer, '\0', 256);
        strcpy(buffer, "0");
        stat = files_puts(file, buffer, strlen(buffer));
        check_return(stat, file);

        /* line 3 - effective baud rate */

        memset(buffer, '\0', 256);
        strcpy(buffer, "0");
        stat = files_puts(file, buffer, strlen(buffer));
        check_return(stat, file);

        /* line 4 - bbs name */

        stat = files_puts(file, humannode, strlen(humannode));
        check_return(stat, file);

        /* line 5 - user record */

        memcpy(buffer, '\0', 256);
        snprintf(buffer, 7, "%d", user->eternal);

        stat = files_puts(file, buffer, strlen(buffer));
        check_return(stat, file);

        /* line 6 - user name */

        if (has_profile(user)) {

            stat = profile_get(profiles, user->profile, &profile);
            check_return(stat, profiles);

            stat = files_puts(file, profile.name, strlen(profile.name));
            check_return(stat, file);

        } else {

            stat = files_puts(filem user->username, strlen(user->username));
            check_return(stat, file);

        }


        /* line 7 - user handle */

        stat = files_puts(filem user->username, strlen(user->username));
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

        stat = files_puts(file, buffer, strlen(buffer));
        check_return(stat, file);

        /* line 9 - time left */

        memset(buffer, '\0', 256);
        snprintf(buffer, 7, "%d", (user->timelimit - timelim));

        stat = files_puts(file, buffer, strlen(buffer));
        check_return(stat, file);

        /* line 10 - graphics mode */
        /* 0 = Ascii, 1 = Ansi, 2 = Avatar, 3 = RIP, 4 = Max Graphics */

        memset(buffer, '\0', 256);
        strcpy(buffer, "1");
        stat = files_puts(file, buffer, strlen(buffer));
        check_return(stat, file);

        /* line 11 - node number */

        memcpy(buffer, '\0', 256);
        snprintf(buffer, 3, "%d", node->nodenum);

        stat = files_puts(file, buffer, strlen(buffer));
        check_return(stat, file);

        /* all done */

        stat = files_close(file);
        check_return(stat, file);

        stat = files_destroy(file);
        check_return(stat, file);

        exit_when;

    } use {

        stat = ERR;
        copy_error(errors);
        capture_trace(dump);
        clear_error();

    } end_when;

    return stat;

}

static int write_door_sys(room_base_t *room, nodes_base_t *node, user_base_t *user) {

    int stat = OK;
    int exists = 0;
    char buffer[256];
    user_base_t sysop;
    char filename[256];
    files_t *file = NULL;
    int sysopnum = SYSOP;
    profile_base_t profile;
    int mode = (S_IRWXU | S_IRWXG);
    int create = (O_RDWR | O_CREAT);

    when_error_in {

        stat = user_get(users, sysopnum, &sysop);
        check_return(stat, users);

        if (has_profile(user)) {

            stat = profile_get(profiles, user->profile, &profile);
            check_return(stat, profiles);

        }

        memset(filename, '\0', 256);
        strncpy(filename, fnm_build(1, FnmPath, "door", ".sys", node->nodenum, workpath, NULL), 255);

        file = files_create(filename, retries, timeout);
        check_creation(file);

        stat = files_exists(file, &exists);
        check_return(stat, file);

        if (exists) {

            stat = files_unlink(file);
            check_return(stat, file);

        }

        stat = files_open(file, create, mode);
        check_return(stat, file);

        stat = files_set_eol(file, "\012\013");
        check_return(stat, file);

        /* line 1 - com port */

        memset(buffer, '\0', 256);
        strcpy(buffer, "COM0:");
        stat = files_puts(file, buffer, strlen(buffer));
        check_return(stat, file);

        /* line 2 - efective baud rate */

        memset(buffer, '\0', 256);
        strcpy(buffer, "0");
        stat = files_puts(file, buffer, strlen(buffer));
        check_return(stat, file);

        /* line 3 - data bits */

        memset(buffer, '\0', 256);
        strcpy(buffer, "8");
        stat = files_puts(file, buffer, strlen(buffer));
        check_return(stat, file);

        /*line 4 - node number */

        memset(buffer, '\0', 256);
        snprintf(buffer, 3, "%d", node->nodenum);

        stat = files_puts(file, buffer, strlen(buffer)):
        check_return(statm, file);

        /* line 5 - locked baud rate */

        memset(buffer, '\0', 256);
        strcpy(buffer, "19200");
        stat = files_puts(file, buffer, strlen(buffer));
        check_return(stat, file);

        /* line 6 - screen snoop */

        stat = files_puts(file, "Y", 1);
        check_return(stat, file);

        /* line 7 - printer on */

        stat = files_puts(file, "N", 1);
        check_return(stat, file);

        /* line 8 - pager bell */

        stat = files_puts(file, "Y", 1);
        check_return(stat, file);

        /* line 9 - caller alarm */

        stat = files_puts(file, "Y", 1);
        check_return(stat, file);

        /* line 10, username */

        stat = files_puts(files, user->username, strlen(user->username));
        check_return(stat, file);

        /* line 11, location */

        memset(buffer, '\0', 256);

        if (has_profile(user)) {

            snprintf(buffer, 31, "%s, %s", profile.city, profile.state);
            stat = files_puts(file, buffer, strlen(buffer));
            check_return(stat, file);

        } else {

            strcpy(buffer, "unknown");

            stat = files_puts(file, buffer, strlen(buffer));
            check_return(stat, file);

        }

        /* line 12, voice phone */

        if (has_profile(user)) {

            stat = files_put(file, profile.phone, strlen(profile.phone));
            check_return(stat, file);

        } else {

            memset(buffer, '\0', 256);
            strcpy(buffer, "(216) 867-5309");

            stat = files_put(file, buffer, strlen(buffer));
            check_return(stat, file);

        }

        /* line 13, data phone */

        memset(buffer, '\0', 256);
        strcpy(phone, "(216) 606-0842");

        stat = files_put(file, buffer, strlen(buffer));
        check_return(stat, file);

        /* line 14, password */

        stat = files_put(file, "", 1);
        check_return(stat, file);

        /* line 15, security level */

        memset(buffer, '\0', 256);

        if (is_norm(room, user)) {

            strcpy(level, "30");

        } else if (is_aide(room, user)) {

            strcpy(level, "80");

        } else if (is_sysop(room, user)) {

            strcpy(level, "100");

        } else {

            strcpy(buffer, "5");

        }

        stat = files_puts(file, buffer, strlen(buffer));
        check_return(stat, file);

        /* line 16 - total calls */

        memset(buffer, '\0', 256);
        sprintf(buffer, "%d", user->timescalled);

        stat = files_puts(file, buffer, strlen(buffer));
        check_return(stat, file);

        /* line 17, last login date */

        memset(buffer, '\0', 256);
        sprintf(buffer, "%s", gdate(user->lastcall, TRUE));

        stat = files_puts(file, buffer, strlen(buffer));
        check_return(stat, file);

        /* line 18, time left in seconds */

        memset(buffer, '\0', 256);
        snprintf(buffer, 7, "%d", ((user->timelimit - timelim) * 60));

        stat = files_puts(file, buffer, strlen(buffer));
        check_return(stat, file);

        /* line 19, time left in minutes */

        memset(buffer, '\0', 256);
        snprintf(buffer, 7, "%d", (user->timelimit - timelim));

        stat = files_puts(file, buffer, strlen(buffer));
        check_return(stat, file);

        /* line 20, graphics GR, RIP, NG */

        memset(buffer, '\0', 256);
        strcpy(buffer, "GR");

        stat = files_puts(file, buffer, strlen(buffer));
        check_return(stat, file);

        /* line 21, number of screen rows */

        memset(buffer, '\0', 256);
        snprintf(buffer, 3, "%d", (user->rows));

        stat = files_puts(file, buffer, strlen(buffer));
        check_return(stat, file);

        /* line 22, user mode, always N */

        stat = files_puts(file, "N", 1);
        check_return(stat, file);

        /* line 23, always blank */

        stat = files_puts(file, "", 1);
        check_return(stat, file);

        /* line 24, always blank */

        stat = files_puts(file, "", 1);
        check_return(stat, file);

        /* line 25, expiration date */

        memset(buffer, '\0', 256);
        strcpy(buffer, "12-31-1969");

        stat = files_puts(file, buffer, strlen(buffer));
        check_return(stat, file);

        /* line 26, user number */

        memset(buffer, '\0', 256);
        sprintf(buffer, "%d", user->eternal);

        stat = files_puts(file, buffer, strlen(buffer));
        check_return(stat, file);

        /* line 27, transfer protocol */

        stat = files_puts(file, "", 1);
        check_return(stat, file);

        /* line 28, uploads */

        stat = files_puts(file, "0", 1);
        check_return(stat, file);

        /* line 29, downloads */

        stat = files_puts(file, "0", 1);
        check_return(stat, file);

        /* line 30, upload kbs today */

        stat = files_puts(file, "0", 1);
        check_return(stat, file);

        /* line 31, download kbs today */

        stat = files_puts(file, "0", 1);
        check_return(stat, file);

        /* line 32, date of birth */

        memset(buffer, '\0', 256);
        strcpy(buffer, "12-31-1969");

        stat = files_puts(file, buffer, strlen(buffer));
        check_return(stat, file);

        /* line 33, path to userbase */

        stat = files_puts(file, "", 1);
        check_return(stat, file);

        /* line 34, path to message base */

        stat = files_puts(file, "", 1);
        check_return(stat, file);

        /* line 35, sysop name */

        stat = files_puts(file, sysop.name, strlen(sysop.name));
        check_return(stat, file);

        /* line 36, user handle */

        stat = files_puts(file, user->username, strlen(user->username));
        check_return(stat, file);

        /* line 37, next event time */

        memset(buffer, '\0', 256);
        strcpy(buffer, "none");

        stat = files_puts(file, buffer, strlen(buffer));
        check_return(stat, file);

        /* line 38, error free connect */

        stat = files_puts(file, "Y", 1);
        check_return(stat, file);

        /* line 39, always N */

        stat = files_puts(file, "N", 1);
        check_return(stat, file);

        /* line 40, always Y */

        stat = files_puts(file, "Y", 1);
        check_return(stat, file);

        /* line 41, default text colors */

        stat = files_puts(file, "7", 1);
        check_return(stat, file);

        /* line 42, always 0 */

        stat = files_puts(file, "0", 1);
        check_return(stat, file);

        /* line 43, last new files scan date */

        memset(buffer, '\0', 256);
        sprintf(buffer, "%s", gdate(user->lastcall, TRUE));

        stat = files_puts(file, buffer, strlen(buffer));
        check_return(stat, file);

        /* line 44, time of this call */

        memset(buffer, '\0', 256);
        sprintf(buffer, "%s", str_time_hm(user->lastcall, TRUE));

        stat = files_puts(file, buffer, strlen(buffer));
        check_return(stat, file);

        /* line 45, time of last call */

        memset(buffer, '\0', 256);
        sprintf(buffer, "%s", gdate(user->lastcall, TRUE));

        stat = files_puts(file, buffer, strlen(buffer));
        check_return(stat, file);

        /* line 46, always 32768 */

        memset(buffer, '\0', 256);
        strcpy(buffer, "32768");

        stat = files_puts(file, buffer, strlen(buffer));
        check_return(stat, file);

        /* line 47, downloads today */

        stat = files_puts(file, "0", 1);
        check_return(stat, file);

        /* line 48, uploads today */

        stat = files_puts(file, "0", 1);
        check_return(stat, file);

        /* line 49, downloads to day */

        stat = files_puts(file, "0", 1);
        check_return(stat, file);

        /* line 50, comment */

        stat = files_puts(file, "", 1);
        check_return(stat, file);

        /* line 51, always 0 */

        stat = files_puts(file, "0", 1);
        check_return(stat, file);

        /* line 52, number of messages posted */

        stat = files_puts(file, "0", 1);
        check_return(stat, file);

        /* all done */

        stat = files_close(file);
        check_return(stat, file);

        stat = files_destroy(file);
        check_return(stat, file);

        exit_when;

    } use {

        stat = ERR;
        copy_error(errors);
        capture_trace(dump);
        clear_error();

    } end_when;

    return stat;

}

static int spawn(char *mandato) {
    
    int	    rc, status;
    pid_t   pid;
    char    *argv[4];
    
    if (mandato == NULL)
	return 1;   /* Prevent running a shell  */

    Syslog('+', "Execve: /bin/sh -c %s", mandato);
    pid = fork();
    if (pid == -1)
	return 1;
    if (pid == 0) {
	msleep(150);
	argv[0] = (char *)"sh";
	argv[1] = (char *)"-c";
	argv[2] = mandato;
	argv[3] = 0;
	execve("/bin/sh", argv, environ);
	exit(MBERR_EXEC_FAILED);
    }
    e_pid = pid;

    do {
	rc = waitpid(pid, &status, 0);
	e_pid = 0;
    } while (((rc > 0) && (rc != pid)) || ((rc == -1) && (errno == EINTR)));

    switch(rc) {
	case -1:
		WriteError("$Waitpid returned %d, status %d,%d", rc,status>>8,status&0xff);
		return -1;
	case 0:
		return 0;
	default:
		if (WIFEXITED(status)) {
		    rc = WEXITSTATUS(status);
		    if (rc) {
			WriteError("Exec_nosuid: returned error %d", rc);
			return rc;
		    }
		}
		if (WIFSIGNALED(status)) {
		    rc = WTERMSIG(status);
		    WriteError("Wait stopped on signal %d", rc);
		    return rc;
		}
		if (rc)
		    WriteError("Wait stopped unknown, rc=%d", rc);
		return rc;      
	}
	return 0;

}

int bbs_subsys_menu(subsys_t *subsys, room_base_t *room, error_trace_t *errors) {

    int idx = 0;
    theme_t theme;
    int stat = OK;
    int width = 0;
    int count = 6;
    char title[40];
    int height = 0;
    int startx = 0;
    int starty = 0;
    error_trace_t error;
    window_t *win = NULL;
    char *help = "subsys";

    int list_size = 0;
    component_t *vline = NULL;
    component_t *bmenu = NULL;
    menus_list_t *list = NULL;

    /* theme.attribute  = A_NORMAL; */
    /* theme.foreground = BROWN; */
    /* theme.background = BLACK; */

fprintf(stderr, "entering bbs_subsys_menu()\n");

    when_error_in {

        old = FALSE;
        forward = TRUE;

        memset(title, '\0', 40);
        snprintf(title, 38, "Sub System: %s", room->name); 

        width  = (getmaxx(stdscr) - 4);
        height = (getmaxy(stdscr) - 7);
        startx = (getbegx(stdscr) + 1);
        starty = (getbegy(stdscr) + 4);

        errno = 0;
        list = calloc(count, sizeof(menus_list_t));
        if (list == NULL) cause_error(errno);

        SET_MENU(list[idx], "Forward", "forward retrieval of messages", MENUS_T_ITEM, NULL, 0, set_forward);
        idx++; SET_MENU(list[idx], "New", "read new messages", MENUS_T_ITEM, jam, sizeof(jam_t), bbs_new_msgs);
        idx++; SET_MENU(list[idx], "Old", "read old messages", MENUS_T_ITEM, jam, sizeof(jam_t), bbs_old_msgs);
        idx++; SET_MENU(list[idx], "Write", "create a new message", MENUS_T_ITEM, NULL, 0, print_result);
        idx++; SET_MENU(list[idx], "Reverse", "reverse retrieval of messages", MENUS_T_ITEM, NULL, 0, set_reverse);
        idx++; SET_MENU(list[idx], "Help", "help with messages", MENUS_T_ITEM, help, strlen(help), bbs_help);

        if (is_aide(room, &useron)) {

            errno = 0;
            count += 4;
            list = realloc(list, count * sizeof(menus_list_t));
            if (list == NULL) cause_error(errno);

            idx++; SET_MENU(list[idx], "-Aide--", "aide functions", MENUS_T_SEPERATOR, NULL, 0, NULL);
            idx++; SET_MENU(list[idx], "Delete", "delete empty rooms", MENUS_T_ITEM, NULL, 0, print_result);
            idx++; SET_MENU(list[idx], "Edit", "edit room", MENUS_T_ITEM, NULL, 0, print_result);
            idx++; SET_MENU(list[idx], "Kill", "remove this room", MENUS_T_ITEM, NULL, 0, print_result);

        }

        if (is_sysop(room, &useron)) {

            errno = 0;
            count += 3;
            list = realloc(list, count * sizeof(menus_list_t));
            if (list == NULL) cause_error(errno);

            idx++; SET_MENU(list[idx], "-Sysop-", "sysop functions", MENUS_T_SEPERATOR, NULL, 0, NULL);
            idx++; SET_MENU(list[idx], "Aide", "set/clear aide privileges", MENUS_T_ITEM, NULL, 0, print_result);
            idx++; SET_MENU(list[idx], "Disable", "disable a user account", MENUS_T_ITEM, NULL, 0, print_result);

        }

        list_size = ((idx + 1) * sizeof(menus_list_t));

        win = base_window(title, startx, starty, height, width);
        check_creation(win);

        bmenu = list_menu_create(win, 0, 0, height - 2, 9, 1, bbs_send_message, list, list_size);
        check_creation(bmenu);

        vline = vline_create(win, 0, 9, height - 2);
        check_creation(vline);

        stat = window_add(win, bmenu);
        check_return(stat, win);

        stat = window_add(win, vline);
        check_return(stat, win);

        stat = window_set_tab(win, 1);
        check_return(stat, win);

        stat = workbench_add(workbench, win);
        check_return(stat, workbench);

        exit_when;

    } use {

        stat = ERR;
        copy_error(errors);
        capture_trace(dump);
        clear_error();

    } end_when;

fprintf(stderr, "leaving bbs_subsys_menu() - stat: %d\n", stat);
    return stat;

}

