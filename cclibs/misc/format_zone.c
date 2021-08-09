
/*---------------------------------------------------------------------------*/
/*  Copyright (c) 2000 by Kevin L. Esteb                                     */
/*                                                                           */
/*  Permission to use, copy, modify, and distribute this software and its    */
/*  documentation for any purpose and without fee is hereby granted,         */
/*  provided that the above copyright notice appears in all copies. The      */
/*  author makes no representations about the suitability of this software   */
/*  for any purpose. It is provided "as is" without express or implied       */
/*  warranty.                                                                */
/*---------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gpl/ansi_setup.h"

/*-Prototypes-----------------------------------------------------------*/

extern char *trim(char *);
extern char *rtrim(char *);
extern char *strrev(char *);
extern char *strupr(char *);
extern int  pos(char *, char *, int);
extern char *left(const char *, int);
extern char *mid(const char *, int, int);
extern char *convert_zone_overpunch(char *);
extern void *xmalloc(int);

static int  float_test(int, char *);
static void get_format_character(int, char *, char *);
static void get_zone_character(int, char *, char *, int *);

/*----------------------------------------------------------------------*/

/**/

char *format_zone(char *zone, char *fmt) {
/*
 * Function: format_zone.c
 * Version : 1.0
 * Created : 11-Jul-2000
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    This routine takes a zoned numeric string as input and formats 
 *    an output ascii string depending on a the format string. Special 
 *    characters in format string are the following.
 *
 *       Z - If a Z matches a leading zero in the data, it is
 *           replaced by a blank. If not, Z it is replaced by a
 *           digit from the data.
 *
 *       9 - Each 9 is replaced by one digit from the data.
 *
 *       * - If an asterisk matches a leading zero in the data a
 *           * is placed in that character position. If not,
 *           it is replaced by a digit from the data.
 *
 *       , - If all the digits to the left of the comma are
 *           suppressed zeros, the comma is replaced by a
 *           blank. If not, a comma is inserted in the
 *           character position.
 *
 *       - - If only one minus sign is specified, it is replaced
 *           by either a blank (if the data is positive) or
 *           a minus sign (if it is negative). If more then
 *           one minus sign is specified (such as --), the
 *           minus sign is a floating character.
 *
 *       + - If only one plus sign is specified, it is replaced
 *           by either a plus sign (if the data is positive)
 *           or a minus sign (if it is negative). If more
 *           than one plus sign is specified (such as ++),
 *           the sign (plus or minus) is a floating character.
 *
 *       $ - If only on dollar sign is speciified, it is replaced
 *           by a $ in that character position. If more than
 *           one dollar sign is specified (such as $$), the
 *           dollar sign is a floating character.
 *
 *      CR - If the data is negative, the letters CR are
 *           inserted. If the data is positive, CR is
 *           replaced by 2 blanks. (An edit string can
 *           contain only one CR.)
 *
 *      DB - If the data's content is positive, the letters DB
 *           are inserted. If the data is negative, DB is
 *           replaced by 2 blanks. (An edit string can
 *           contain only on DB.)
 *
 *     < > - If the data is negative, the < and > are inserted
 *           on both sides of the edit string. If the data
 *           is positive, < and > is replaced by 1 blank each.
 *           (An edit string can contain only one < > pair.)
 *
 *      Note: all other characters are inserted into that
 *            character position. (e.a. %,/,.,0, ...)
 *                 e.a. $ZZ,ZZ9.99-
 *                      ***,**9.99CR
 *                      <+99999.99>
 *                      XXX-XX-XXXX
 *  
 *      Note: The string returned must be freed with free().
 *
 * Modification History
 *
 * Variables Used
 */

    int ascii; 
    int zone_len; 
    int format_len; 
    int float_done; 
    int leading_zero; 
    int zone_position; 
    int format_position;

    int append_sw = FALSE; 
    int negative_sw = FALSE; 

    char *ch = NULL;
    char *text = NULL;
    char *zone_buf = NULL;

    char buffer[256];

/*
 * Main part of function.
 */

    trim(fmt);
    format_len = strlen(fmt);
    if (format_len == 0) return(NULL);  /* No Format !!!                */

    strupr(fmt);
    text = xmalloc(format_len + 1);

    /* Initial setup.                                                   */

    ch = left(zone, 1);
    ascii = *(ch);

    if ((ascii >= 'p') && (ascii <= 'y')) {

        append_sw = TRUE;
        negative_sw = TRUE;
        zone_buf = convert_zone_overpunch(zone);

    } else zone_buf = strdup(zone);

    /* Sanity check...                                                  */

    zone_len = strlen(zone_buf);

    if (zone_len > format_len) {

        free(text);
        free(zone_buf);
        return(NULL);

    }

    /* Find beginning of the leading zeros.                             */

    leading_zero = pos(zone_buf, "0", 1) - 1;
    if (leading_zero < 0) leading_zero = 0;

    /* Start the main loop.                                             */

    zone_position = zone_len;

    for (format_position = format_len; 
         format_position > 0; 
         format_position--) {

        get_format_character(format_position, fmt, buffer);
        ascii = *(buffer);

        switch (ascii) {
            case 'Z':
                get_zone_character(zone_position, zone_buf, buffer, &leading_zero);
                if (strcmp(buffer, "0") == 0) {

                    if (format_position >= leading_zero) strcpy(buffer, " ");

                } 
                if (strcmp(buffer, "-") == 0) {

                    if (negative_sw) strcpy(buffer, " ");

                }
                zone_position--;
                break;

            case '*':
                get_zone_character(zone_position, zone_buf, buffer, &leading_zero);
                if (strcmp(buffer, "0") == 0) {

                    if (format_position >= leading_zero) strcpy(buffer, "*");

                }
                if (strcmp(buffer, "-") == 0) {

                    if (negative_sw) strcpy(buffer, "*");

                }
                zone_position--;
                break;

            case ',':
                get_format_character(format_position - 1, fmt, buffer);
                if (strcmp(buffer, "Z") == 0) {

                    if (zone_position > 0) {

                        strcpy(buffer, ",");

                    } else strcpy(buffer, " ");

                } else if (strcmp(buffer, "*") == 0) {

                    if (zone_position > 0) {

                        strcpy(buffer, ",");

                    } else strcpy(buffer, "*");

                } else {

                    if (format_position >= leading_zero) {

                        strcpy(buffer, ",");

                    } else strcpy(buffer, " ");

                }
                break;

            case '9':
                get_zone_character(zone_position, zone_buf, buffer, &leading_zero);
                zone_position--;
                if (strcmp(buffer, "-") == 0) {

                    if (negative_sw) strcpy(buffer, " ");

                }
                break;

            case '-':
                if (format_position >= leading_zero) {

                    if (float_test(format_position, fmt) & float_done) {

                        strcpy(buffer, " ");

                    } else {

                        if (negative_sw) {

                            if (zone_position > 1) {

                                strcpy(buffer, "-");

                            } else {

                                rtrim(text);
                                strcpy(buffer, "- ");

                            }
                            append_sw = FALSE;

                        } else strcpy(buffer, " ");

                        float_done = TRUE;

                    }

                }
                break;

            case '+':
                if (format_position >= leading_zero) {

                    if (float_test(format_position, fmt) & float_done) {

                        strcpy(buffer, " ");

                    } else {

                        if (negative_sw) {

                            if (zone_position > 1) {

                                strcpy(buffer, "-");

                            } else {

                                rtrim(text);
                                strcpy(buffer, "- ");

                            }
                            append_sw = FALSE;

                        } else {

                            if (zone_position > 1) {

                                strcpy(buffer, "+");

                            } else {

                                rtrim(text);
                                strcpy(buffer, "+ ");

                            }

                        }

                        float_done = TRUE;

                    }

                }
                break;

            case '$':
                if (format_position >= leading_zero) {

                    if (float_test(format_position, fmt) & float_done) {

                        strcpy(buffer, " ");

                    } else {

                        strcpy(buffer, "$");
                        float_done = TRUE;

                    }

                }
                break;

            case 'R':
                if (negative_sw) {

                    strcpy(buffer, "R");

                } else strcpy(buffer, " ");
                break;

            case 'C':
                if (negative_sw) {

                    strcpy(buffer, "C");
                    append_sw = FALSE;

                } else strcpy(buffer, " ");
                break;

            case 'D':
                if (negative_sw) {

                    strcpy(buffer, "D");

                } else strcpy(buffer, " ");
                break;

            case 'B':
                if (negative_sw) {

                    strcpy(buffer, "B");
                    append_sw = FALSE;

                } else strcpy(buffer, "  ");
                break;

            case '<':
                if (negative_sw) {

                    strcpy(buffer, "<");
                    append_sw = FALSE;

                } else strcpy(buffer, " ");
                break;

            case '>':
                if (negative_sw) {

                    strcpy(buffer, ">");

                } else strcpy(buffer, " ");
                break;

            default:
                break;

        }

        strcat(text, buffer);

    }

    if (append_sw) {

        sprintf(buffer, "-%s", text);
        strcpy(text, buffer);

    }

    free(zone_buf);
    strrev(text);

    return(text);

}

/**/

static int float_test(int position, char *fmt) {
/*
 * Function: float_test.c
 * Version : 1.0
 * Created : 11-Jul-2000
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    Test if the current format char is a float char. This is true
 *    if the next or last format char was a float.
 *
 * Modification History
 *
 * Variables Used
 */

    char *ch = NULL;
    char *ch1 = NULL;
    char *ch2 = NULL;

    int ascii;
    int stat = FALSE;

/*
 * Main part of function.
 */

    ch = mid(fmt, position, 1);
    ascii = *(ch);

    switch (ascii) {
        case '-':
            ch1 = mid(fmt, position + 1, 1);
            ch2 = mid(fmt, position - 1, 1);
            if ((strcmp(ch1, "-") == 0) ||
                (strcmp(ch2, "-") == 0)) stat = TRUE;
            break;

        case '+':
            ch1 = mid(fmt, position + 1, 1);
            ch2 = mid(fmt, position - 1, 1);
            if ((strcmp(ch1, "+") == 0) ||
                (strcmp(ch2, "+") == 0)) stat = TRUE;
            break;

        case '$':
            ch1 = mid(fmt, position + 1, 1);
            ch2 = mid(fmt, position - 1, 1);
            if ((strcmp(ch1, "$") == 0) ||
                (strcmp(ch2, "$") == 0)) stat = TRUE;
            break;

        default:
            break;

    }

    free(ch);
    free(ch1);
    free(ch2);

    return(stat);

}

/**/

static void get_zone_character(int position, char *zone, char *buffer, 
                               int *leading_zero) {
/*
 * Function: get_zone_character.c
 * Version : 1.0
 * Created : 11-Jul-2000
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    Gets next zone character, back to front. If out of characters
 *    then zero returned. The leading_zero is set if leading zero passed.
 *
 * Modification History
 *
 * Variables Used
 */

    char *ch = NULL;

/*
 * Main part of function.
 */

    if (position < 1) {

        strcpy(buffer, "0");

    } else {

        ch = mid(zone, position, 1);

        if (strcmp(ch, " ") == 0) {

            strcpy(buffer, "0");
            if (*leading_zero <= position) *leading_zero = position;

        } else strcpy(buffer, ch);

        free(ch);

    }

}

/**/

static void get_format_character(int position, char *fmt, char *buffer) {
/*
 * Function: get_format_character.c
 * Version : 1.0
 * Created : 11-Jul-2000
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    Gets next format character, back to front. If out of characters
 *    then a blank is returned.
 *
 * Modification History
 *
 * Variables Used
 */

    char *ch = NULL;

/*
 * Main part of function.
 */

    if (position < 1) {

        strcpy(buffer, " ");

    } else {

        ch = mid(fmt, position, 1);
        strcpy(buffer, ch);
        free(ch);

    }
   
}

