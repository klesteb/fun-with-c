
/*---------------------------------------------------------------------------*/
/*  Copyright (c) 1999 by Kevin L. Esteb                                     */
/*                                                                           */
/*  Permission to use, copy, modify, and distribute this software and its    */
/*  documentation for any purpose and without fee is hereby granted,         */
/*  provided that the above copyright notice appears in all copies. The      */
/*  author makes no representations about the suitability of this software   */
/*  for any purpose. It is provided "as is" without express or implied       */
/*  warranty.                                                                */
/*---------------------------------------------------------------------------*/

#ifndef DATES_H
#define DATES_H 1

#ifndef TRUE
#define TRUE (-1)
#define FALSE (0)
#endif

/*----------------------------------------------------------------------*/
/* Julian day routines                                                  */
/*----------------------------------------------------------------------*/

/* 
 * Dates are represented as julian day number, stored as long integers
 * (at least 32 bits).
 */

typedef enum {
    fmt_start = -1,
    fmt_dd_mmm_yyyy,            /* DEC's dd-mmm-yyyy format             */
    fmt_end
} date_fmt;

typedef long DATE;

DATE julian_date(int day, int month, int year);
DATE sys_date(void);
DATE easter(int year);
DATE today(void);
DATE add_days(DATE jdate, int days);
DATE sub_days(DATE jdate, int days);
int  day_of_week(DATE jdate);
int  day_of_year(DATE jdate);
int  week_of_year(DATE jdate);
int  month_of_year(DATE jdate);
int  isleap(DATE jdate);
int  valid_date(DATE jdate);
void calendar_date(DATE jdate, int *day, int *month, int *year);
char *format_date(DATE jdate, date_fmt);
char *moon_phase(DATE jdate);

#endif

