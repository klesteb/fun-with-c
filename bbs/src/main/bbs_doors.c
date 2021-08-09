
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

extern int print_result();

/* local items ---------------------------------------------------------- */

static int write_doorinfo_def(room_base_t *room, nodes_base_t *node, user_base_t *user) {

    int stat = OK;
    char temp2[37];
    int exists = 0;
    user_base_t sysop;
    char filename[256];
    char *temp3 = NULL;
    files_t *file = NULL;
    int sysopnum = SYSOP;
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

        char *comport = "COM0";
        stat = files_puts(file, comport, strlen(comport));
        check_return(stat, file);

        /* line 5 - connection speed */

        char *connection = "19200 BAUD,N,8,1";
        stat = files_puts(file, connection, strlen(connection));
        check_return(stat, file);

        /* line 6 - networked ?? */

        char *networked = "0";
        stat = files_puts(filem networked, strlen(networked));
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

        char *location = "unknown";
        if (bit_test(user->flags, US_PROFILE)) {

            stat = files_puts(file, location, strlen(location));
            check_return(stat, file);

        } else {

            stat = files_puts(file, location, strlen(location));
            check_return(stat, file);

        }

        /* line 10 - emulation "0" if TTY, or "1" if ANSI. */

        char *emulation = "1";
        stat = files_puts(file, emulation, strlen(emulation));
        check_return(stat, file);

        /* line 11 - security level */

        char level[5];
        memset(level, '\0', 5);
        strcpy(level, "5");

        if (is_norm(room, user)) {

            strcpy(level, "30");

        } else if (is_aide(room, user)) {

            strcpy(level, "80");

        } else if (is_sysop(room, user)) {

            strcpy(level, "100");

        }

        stat = files_puts(file, level, strlen(level));
        check_return(stat, file);

        /* line 12 - time left */

        char timeleft[8];
        memset(timeleft, '\0', 8);
        snprintf(timeleft, 7, (user->timelimit - timelim));

        stat = files_puts(file, timeleft, strlen(timeleft));
        check_return(stat, file);

        /* line 13 - FOSSIL driver "-1" if external, "0" if internal */

        char *fossil = "-1";
        stat = files_puts(file, fossil, strlen(fossil));
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
    user_base_t sysop;
    char filename[256];
    files_t *file = NULL;
    int sysopnum = SYSOP;
    int mode = (S_IRWXU | S_IRWXG);
    int create = (O_RDWR | O_CREAT);

    when_error_in {

        stat = user_get(users, sysopnum, &sysop);
        check_return(stat, users);

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

        char *comtype = "0";
        stat = files_puts(file, comtype, strlen(comtype));
        check_return(stat, file);

        /* line 2 - com port, number or socket */

        char *comport = "0";
        stat = files_puts(file, comport, strlen(comport));
        check_return(stat, file);

        /* line 3 - effective baud rate */

        char *baud = "0";
        stat = files_puts(file, baud, strlen(baud));
        check_return(stat, file);

        /* line 4 - bbs name */

        stat = files_puts(file, humannode, strlen(humannode));
        check_return(stat, file);

        /* line 5 - user record */

        char usernum[8];
        memcpy(usernum, '\0', 8);
        snprintf(usernum, 7, "%s", user->eternal);

        stat = files_puts(file, usernum strlen(usernum));
        check_return(stat, file);

        /* line 6 - user name */

        stat = files_puts(filem user->username, strlen(user->username));
        check_return(stat, file);

        /* line 7 - user handle */

        stat = files_puts(filem user->username, strlen(user->username));
        check_return(stat, file);

        /* line 8 - security level */

        char level[5];
        memset(level, '\0', 5);
        strcpy(level, "5");

        if (is_norm(room, user)) {

            strcpy(level, "30");

        } else if (is_aide(room, user)) {

            strcpy(level, "80");

        } else if (is_sysop(room, user)) {

            strcpy(level, "100");

        }

        stat = files_puts(file, level, strlen(level));
        check_return(stat, file);

        /* line 9 - time left */

        char timeleft[8];
        memset(timeleft, '\0', 8);
        snprintf(timeleft, 7, (user->timelimit - timelim));

        stat = files_puts(file, timeleft, strlen(timeleft));
        check_return(stat, file);

        /* line 10 - graphics mode */
        /* 0 = Ascii, 1 = Ansi, 2 = Avatar, 3 = RIP, 4 = Max Graphics */

        char *graphics = "1";
        stat = files_puts(file, graphics, strlen(graphics));
        check_return(stat, file);

        /* line 11 - node number */

        char nodenum[4];
        memcpy(nodenum , '\0', 4);
        snprintf(nodenum, 3, "%s", node->nodenum);

        stat = files_puts(file, nodenum, strlen(nodenum));
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
    user_base_t sysop;
    char filename[256];
    files_t *file = NULL;
    int sysopnum = SYSOP;
    int mode = (S_IRWXU | S_IRWXG);
    int create = (O_RDWR | O_CREAT);

    when_error_in {

        stat = user_get(users, sysopnum, &sysop);
        check_return(stat, users);

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
        
        char *comport = "COM0:";
        stat = files_puts(file, comport, strlen(comport));
        check_return(stat, file);
        
        /* line 2 - efective baud rate */
        
        char *baud = "0";
        stat = files_puts(file, baud, strlen(baud));
        check_return(stat, file);
        
        /* line 3 - data bits */
        
        char *bits = "8";
        stat = files_puts(file, bits, strlen(bits));
        check_return(stat, file);

        /*line 4 - node number */
        
        char nodenum[8];
        memset(nodenum, '\0', 8);
        snprintf(nodenum, 7 "%s", node->nodenum);
        
        stat = files_puts(file, nodenum, strlen(nodenum)):
        check_return(statm, file);

        /* line 5 - locked baud rate */
        
        stat = files_puts(file, baud, strlen(baud));
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
        
        char *location = "unknown";
        if (bit_test(user->flags, US_PROFILE)) {

            stat = files_puts(file, location, strlen(location));
            check_return(stat, file);

        } else {

            stat = files_puts(file, location, strlen(location));
            check_return(stat, file);

        }

        /* line 12, voice phone */
        

    fprintf(fp, "%s\r\n", exitinfo.sVoicePhone);
    fprintf(fp, "%s\r\n", exitinfo.sDataPhone);
    fprintf(fp, "%s\r\n", exitinfo.Password);
    fprintf(fp, "%d\r\n", exitinfo.Security.level);
    fprintf(fp, "%d\r\n", exitinfo.iTotalCalls);
    fprintf(fp, "%s\r\n", Gdate(exitinfo.tLastLoginDate, Y2Kdoorsys));
    fprintf(fp, "%d\r\n", exitinfo.iTimeLeft * 60);    /* Seconds    */
    fprintf(fp, "%d\r\n", exitinfo.iTimeLeft);    /* Minutes    */
    fprintf(fp, "GR\r\n");        /* Graphics GR,RIP,NG */
    fprintf(fp, "%d\r\n", rows);
    fprintf(fp, "N\r\n");        /* User mode, always N    */
    fprintf(fp, "\r\n");        /* Always blank        */
    fprintf(fp, "\r\n");        /* Always blank        */
    fprintf(fp, "%s\r\n", Rdate(exitinfo.sExpiryDate, Y2Kdoorsys));
    fprintf(fp, "%d\r\n", grecno);    /* Users recordnumber    */
    fprintf(fp, "%s\r\n", exitinfo.sProtocol);
    fprintf(fp, "%d\r\n", exitinfo.Uploads);
    fprintf(fp, "%d\r\n", exitinfo.Downloads);
    fprintf(fp, "%d\r\n", LIMIT.DownK); /* FIXME: Download Kb today */
    fprintf(fp, "%d\r\n", LIMIT.DownK);
    fprintf(fp, "%s\r\n", Rdate(exitinfo.sDateOfBirth, Y2Kdoorsys));
    fprintf(fp, "\r\n");        /* Path to userbase    */
    fprintf(fp, "\r\n");        /* Path to messagebase    */
    fprintf(fp, "%s\r\n", CFG.sysop_name);
    fprintf(fp, "%s\r\n", exitinfo.sHandle);
    fprintf(fp, "none\r\n");    /* Next event time    */
    fprintf(fp, "Y\r\n");        /* Error free connect.    */
    fprintf(fp, "N\r\n");        /* Always N        */
    fprintf(fp, "Y\r\n");        /* Always Y        */
    fprintf(fp, "7\r\n");        /* Default textcolor    */
    fprintf(fp, "0\r\n");        /* Always 0        */
    fprintf(fp, "%s\r\n", Gdate(exitinfo.tLastLoginDate, Y2Kdoorsys)); /* Last newfiles scan date */
    fprintf(fp, "%s\r\n", StrTimeHM(t_start));  /* Time of this call    */
    fprintf(fp, "%s\r\n", LastLoginTime);        /* Time of last call    */
    fprintf(fp, "32768\r\n");    /* Always 32768        */
    fprintf(fp, "%d\r\n", exitinfo.DownloadsToday);
    fprintf(fp, "%d\r\n", exitinfo.UploadK);
    fprintf(fp, "%d\r\n", exitinfo.DownloadK);
    fprintf(fp, "%s\r\n", exitinfo.sComment);
    fprintf(fp, "0\r\n");        /* Always 0        */
    fprintf(fp, "%d\r\n\032", exitinfo.iPosted);

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

