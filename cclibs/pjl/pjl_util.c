
/*----------------------------------------------------------------------*/
/* File: pjl_utils.c                                                    */
/* Date: 30-Oct-2000                                                    */
/* By  : Kevin Esteb                                                    */
/* Ver : 0.01                                                           */
/*                                                                      */
/* This library needs to be used with the lfn_util.c library for these  */
/* functions to work correctly.                                         */
/*                                                                      */
/*----------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>

#include "lfn_util.h"
#include "pjl_util.h"
#include "que_util.h"
#include "vms.h"

#define TIMEOUT 2.0

/*-Prototypes-----------------------------------------------------------*/

static int get_response(LfnStream stream, queue *config);

extern char *right(const char *, int);
extern int  stricmp(const char *, const char *);
extern int  pos(int, const char *, const char *);
extern int  strnicmp(const char *, const char *, int);

/*-Global-Variables-----------------------------------------------------*/

static queue ustatus;               /* The ustatus of the printer.      */
static queue variables;             /* Info variables from the printer. */
static queue printer_cfg;           /* Config info from the printer.    */

/*----------------------------------------------------------------------*/

/*␌*/

int pjl_startup(LfnStream stream) {
/*
 * Function: pjl_startup.c
 * Version : 1.0
 * Created : 09-Nov-2000
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    This function will setup the pjl communications with the printer. It
 *    also loads the current printer configuration into static memory.
 *
 * Modification History
 *
 * Variables Used
 */

    int stat;

/*
 * Main part of function.
 */

    que_init(&ustatus);
    que_init(&variables);
    que_init(&printer_cfg);

    /* Put us into command mode.                                        */

    stat = lfn_putline(stream, TIMEOUT, "\033%%-12345X@PJL \r\n");
    if (stat != 0) return(stat);

    /* Syncronize with the printer.                                     */

    stat = pjl_echo(stream);
    if (stat != 0) return(stat);

    /* Ask for the printer configuration.                               */

    stat = lfn_putline(stream, TIMEOUT, "@PJL INFO CONFIG \r\n");
    if (stat != 0) return(stat);

    stat = get_response(stream, &printer_cfg);
    if (stat != 0) return(stat);

    /* Syncronize with the printer.                                     */

    stat = pjl_echo(stream);
    if (stat != 0) return(stat);

    /* Ask for the environment variables.                               */

    stat = lfn_putline(stream, TIMEOUT, "@PJL INFO VARIABLES \r\n");
    if (stat != 0) return(stat);

    stat = get_response(stream, &variables);
    if (stat != 0) return(stat);

    /* Syncronize with the printer.                                     */

    stat = pjl_echo(stream);
    if (stat != 0) return(stat);

    /* Ask for the ustatus.                                             */

    stat = lfn_putline(stream, TIMEOUT, "@PJL INFO USTATUS \r\n");
    if (stat != 0) return(stat);

    stat = get_response(stream, &ustatus);

    return(stat);

}

/*␌*/

int pjl_shutdown(LfnStream stream) {
/*
 * Function: pjl_startup.c
 * Version : 1.0
 * Created : 09-Nov-2000
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    This function will terminate the pjl stream with the printer.
 *    It will also free up the memory used by the printer configuration.
 *
 * Modification History
 *
 * Variables Used
 */

    int stat;
    char *y = NULL;

/*
 * Main part of function.
 */

    /* Terminate our pjl connection to the printer.                     */

    stat = lfn_putline(stream, TIMEOUT, "\033%%-12345X");

    /* Free up the printer configuration information.                   */

    if (!que_empty(&printer_cfg)) {

        while ((y = que_pop_tail(&printer_cfg)) != NULL) {

            free(y);

        }

    }

    /* Free up the printer variable information.                        */

    if (!que_empty(&variables)) {

        while ((y = que_pop_tail(&variables)) != NULL) {

            free(y);

        }

    }

    /* Free up the printer ustatus information.                         */

    if (!que_empty(&ustatus)) {

        while ((y = que_pop_tail(&ustatus)) != NULL) {

            free(y);

        }

    }

    return(stat);

}

/*␌*/

int pjl_jobname(LfnStream stream, char *jobname) {
/*
 * Function: pjl_jobname
 * Version : 1.0
 * Created : 09-Nov-2000
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    This function will give the pjl job a name.
 *
 * Modification History
 *
 * Variables Used
 */

    int stat;

/*
 * Main part of function.
 */

    stat = lfn_putline(stream, TIMEOUT, "@PJL JOB NAME = \"%s\" \r\n", jobname);

    return(stat);

}

/*␌*/

int pjl_language(LfnStream stream, char *language) {
/*
 * Function: pjl_language.c
 * Version : 1.0
 * Created : 09-Nov-2000
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    This function will set the default printer control language. It will
 *    check the printer configuration to see if the desired language is
 *    supported. If not a -1 is returned.
 *
 * Modification History
 *
 * Variables Used
 */

    int stat = 0;
    char *buff = NULL;
    char *wanted = "LANGUAGES";

/*
 * Main part of function.
 */

    for (buff = que_first(&printer_cfg);
         buff != NULL;
         buff = que_next(&printer_cfg)) {

        if (strnicmp(buff, wanted, strlen(wanted)) == 0) {

            for (;
                 buff != NULL;
                 buff = que_next(&printer_cfg)) {

                if (buff[0] == '\t') {

                    if (stricmp(language, buff) == 0) {

                        stat = lfn_putline(stream, TIMEOUT,
                                           "@PJL ENTER LANGUAGE = %s \r\n",
                                           language);
                        goto fini;

                    }

                } else {

                    stat = -1;
                    goto fini;

                }

            }

        }

    }

    fini:
    return(stat);

}

/*␌*/

int pjl_ustatusoff(LfnStream stream) {
/*
 * Function: pjl_ustatusoff.c
 * Version : 1.0
 * Created : 09-Nov-2000
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    This function will turn off the pjl ustatus.
 *
 * Modification History
 *
 * Variables Used
 */

    int stat;

/*
 * Main part of function.
 */

    stat = lfn_putline(stream, TIMEOUT, "@PJL USTATUSOFF \r\n");

    return(stat);

}

/*␌*/

int pjl_ustatus(LfnStream stream, char *type, char *action) {
/*
 * Function: pjl_ustatus.c
 * Version : 1.0
 * Created : 09-Nov-2000
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    This function will check the ustatus type and action against the printer
 *    configuration. If the type is not in the configuration a -1 will be
 *    returned. If the action is not defined then a -1 will be returned.
 *
 * Modification History
 *
 * Variables Used
 */

    int stat = 0;
    char *buff = NULL;
    char *wanted = "USTATUS";

/*
 * Main part of function.
 */

    if ((stricmp(action, "ON") != 0) ||
        (stricmp(action, "OFF") != 0) ||
        (stricmp(action, "VERBOSE") != 0)) return(-1);

    for (buff = que_first(&printer_cfg);
         buff != NULL;
         buff = que_next(&printer_cfg)) {

        if (strnicmp(buff, wanted, strlen(wanted)) == 0) {

            for (;
                 buff != NULL;
                 buff = que_next(&printer_cfg)) {

                if (buff[0] == '\t') {

                    if (stricmp(type, buff) == 0) {

                        stat = lfn_putline(stream, TIMEOUT,
                                           "@PJL USTATUS %s = %s \r\n",
                                           type, action);
                        goto fini;

                    }

                } else {

                    stat = -1;
                    goto fini;

                }

            }

        }

    }

    fini:
    return(stat);

}

/*␌*/

int  pjl_rdymsg(LfnStream stream, char *msg) {
/*
 * Function: pjl_rdymsg.c
 * Version : 1.0
 * Created : 09-Nov-2000
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    This function will set the ready message on the printer front panel.
 *    It will search the configuration to find the display size. If one is
 *    not found it will default to 15 characters.
 *
 * Modification History
 *
 * Variables Used
 */

    int stat;
    int len = 15;
    int lines;
    char *buff = NULL;
    char *wanted = "DISPLAY CHARACTER SIZE";

/*
 * Main part of function.
 */

    if (strlen(msg) > 0) {

        for (buff = que_first(&printer_cfg);
             buff != NULL;
             buff = que_next(&printer_cfg)) {

            if (strnicmp(buff, wanted, strlen(wanted)) == 0) {

                len = pos(1, buff, "=");
                lines = atoi(right(buff, len + 1));

                msg[lines] = '\0';

            }

        }

        stat = lfn_putline(stream, TIMEOUT,
                           "@PJL RDYMSG DISPLAY = \"  %s\" \r\n", msg);

    } else {

        stat = lfn_putline(stream, TIMEOUT, "@PJL RDYMSG DISPLAY = \"\" \r\n");

    }

    return(stat);

}

/*␌*/

int pjl_eoj(LfnStream stream, char *jobname) {
/*
 * Function: pjl_eoj.c
 * Version : 1.0
 * Created : 09-Nov-2000
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    This function will signal the pjl stream that the current job is
 *    finished.
 *
 * Modification History
 *
 * Variables Used
 */

    int stat;

/*
 * Main part of function.
 */

    stat = lfn_putline(stream, TIMEOUT, "@PJL EOJ NAME = \"%s\" \r\n", jobname);

    return(stat);

}

/*␌*/

int pjl_echo(LfnStream stream) {
/*
 * Function: pjl_echo.c
 * Version : 1.0
 * Created : 09-Nov-2000
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    This function will send a pjl echo command to syncronize the job stream
 *    with the printer. An echo command will cause the printer to flush the
 *    pjl command buffer. This information can be ignored.
 *
 * Modification History
 *
 * Variables Used
 */

    char *y;
    int stat;
    time_t t;
    struct tm *tb;
    queue junk;
    char buff[1024];

/*
 * Main part of function.
 */

    que_init(&junk);

    t = time(NULL);
    tb = localtime(&t);
    strftime(buff, 127, "@PJL ECHO VMS %d-%b-%Y %T \r\n", tb);

    stat = lfn_putline(stream, TIMEOUT, buff);
    if (stat != 0) return(stat);

    /* Read the response back from the printer.                         */

    stat = get_response(stream, &junk);

    /* Throw the response away. Just sycronizing.                       */

    if (!que_empty(&junk)) {

        while ((y = que_pop_tail(&junk)) != NULL) {

            free(y);

        }

    }

    return(stat);

}

/*␌*/

int pjl_inquire(LfnStream stream, char *variable, char *value, int length) {
/*
 * Function: pjl_echo.c
 * Version : 1.0
 * Created : 09-Nov-2000
 * Author  : Kevin Esteb
 *
 * Description
 *
 *
 * Modification History
 *
 * Variables Used
 */

    int stat;
    int len;
    char *buff = NULL;
    char cmd[1024];
    queue junk;

/*
 * Main part of function.
 */

    que_init(&junk);

    /* Send the inquiry to the printer.                                 */

    sprintf(cmd, "@PJL INQUIRE %s \r\n", variable);
    stat = lfn_putline(stream, TIMEOUT, cmd);
    if (stat != 0) return(stat);

    len = (strlen(cmd) - 3);
    cmd[len] = '\0';

    /* Read the response back from the printer.                         */

    stat = get_response(stream, &junk);
    if (stat != 0) return(stat);

    /* Parse the information from the returned list. The first line    */
    /* will be the pjl command. The second the information wanted.     */

    for (buff = que_first(&junk);
         buff != NULL;
         buff = que_next(&junk)) {

        if (strnicmp(buff, cmd, strlen(cmd)) == 0) {

            buff = que_next(&junk);
            strncpy(value, buff, length);
            value[length] = '\0';
            break;

        }

    }

    /* Free list.                                                       */

    if (!que_empty(&junk)) {

        while ((buff = que_pop_tail(&junk)) != NULL) {

            free(buff);

        }

    }

    return(stat);

}

/*␌*/

void pjl_dump_config(void) {
/*
 * Function: pjl_dump_config.c
 * Version : 1.0
 * Created : 13-Nov-2000
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    This function will dump the saved printer config to stdout.
 *
 * Modification History
 *
 * Variables Used
 */

    char *buff = NULL;

/*
 * Main part of function.
 */

    printf("\nPrinter config:\n\n");

    for (buff = que_first(&printer_cfg);
         buff != NULL;
         buff = que_next(&printer_cfg)) {

        printf("%s\n", buff);

    }

}

/*␌*/

void pjl_dump_ustatus(void) {
/*
 * Function: pjl_dump_ustatus.c
 * Version : 1.0
 * Created : 13-Nov-2000
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    This function will dump the saved ustatus to stdout.
 *
 * Modification History
 *
 * Variables Used
 */

    char *buff = NULL;

/*
 * Main part of function.
 */

    printf("\nPrinter ustatus:\n\n");

    for (buff = que_first(&ustatus);
         buff != NULL;
         buff = que_next(&ustatus)) {

        printf("%s\n", buff);

    }

    printf("\n");

}

/*␌*/

void pjl_dump_variables(void) {
/*
 * Function: pjl_dump_variables.c
 * Version : 1.0
 * Created : 13-Nov-2000
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    This function will dump the saved environment variables to stdout.
 *
 * Modification History
 *
 * Variables Used
 */

    char *buff = NULL;

/*
 * Main part of function.
 */

    printf("\nPrinter environment variables:\n\n");

    for (buff = que_first(&variables);
         buff != NULL;
         buff = que_next(&variables)) {

        printf("%s\n", buff);

    }

    printf("\n");

}

/*␌*/

static int get_response(LfnStream stream, queue *list) {
/*
 * Function: get_response.c
 * Version : 1.0
 * Created : 09-Nov-2000
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    This function will read the response back from the lazer printer.
 *    The response ends with a <FF>. For some reason a HPLJ 4050 places
 *    a <FF> in front of the returned pjl command. This is stripped off.
 *
 * Modification History
 *
 * Variables Used
 */

    int stat;
    int count = 0;
    char *y = NULL;
    char *buff = NULL;

/*
 * Main part of function.
 */

    for (;;) {

        stat = lfn_getline(stream, TIMEOUT, &y);
        switch (stat) {
            case 0:                         /* Normal read.         */
                if (y[0] == '\f') memmove(y, &y[1], strlen(y));
                if (strlen(y) > 1) {
                    buff = strdup(y);
                    que_push_tail(list, buff);
                }
                break;

            case EWOULDBLOCK:               /* Timeout condition.   */
                stat = 0;
                goto fini;
                break;

            default:                        /* Some error occured.  */
                goto fini;
                break;

        }

    }

    fini:
    return(stat);

}

