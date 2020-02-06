
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

#include <time.h>
#include "dates.h"

/*----------------------------------------------------------------------*/

DATE julian_date(int day, int month, int year) {
                        
    int a,b;
    float year_corr;
        
    /* correct for negative year                                        */
        
    year_corr = (year > 0 ? 0.0 : 0.75);
        
    if (month <= 2) {
                
        year--;
        month += 12;
                
    }
        
    b = 0;
        
    /* cope with Gregorian calendar reform                                */
        
    if (year * 10000.0 + month * 100.0 + day >= 15821015.0) {
                
        a = year / 100;
        b = 2 - a + a / 4;
                
    }
        
    return ((DATE)(long)(365.25 * year - year_corr) + 
            (long)(30.6001 * (month + 1)) + day + 1720994L + b);
        
}

/**/

void calendar_date(DATE jdate, int *day, int *month, int *year) {
        
    /* calculate the day, month, and year corresponding to a julian
     * day number. The year will be negative if it is B.C.
     */
        
    long a, b, c, d, e, z, alpha;
        
    z = jdate + 1;
        
    /* cope with the Gregorian calendar reform                          */
        
    if (z < 2299161L) {
                
		a = z;
                
    } else {
                
        alpha = (long)((z - 1867216.25) / 36524.25);
        a = z + 1 + alpha - alpha / 4;
                
    }
        
    b = a + 1524;
    c = (long)((b - 122.1) / 365.25);
    d = (long)(365.25 * c);
    e = (long)((b - d) / 30.6001);
    *day = (int)(b - d - (long)(30.6001 * e));
    *month = (int)((e < 13.5) ? e - 1 : e - 13);
    *year = (int)((*month > 2.5) ? (c - 4716) : (c - 4715));
        
}

DATE easter(int year) {
        
    /* calculate the julian day number of easter for the specified year,
     * which can not be negative.
     */
        
    int a, b, c, d, e, f, g, h, i, k, l, m, n, p, day, month;
        
    if (year >= 1583) {
                
        /* calculate date of easter for a year in the Gregorian calander */
                
        a = year % 19;
        b = year / 100;
        c = year % 100;
        d = b / 4;
        e = b % 4;
        f = (b + 8) / 25;
        g = (b - f + 1) / 3;
        h = (19 * a + b - d - g + 15) % 30;
        i = c / 4;
        k = c % 4;
        l = (32 + 2 * e + 2 * i - h - k) % 7;
        m = (a + 11 * h + 22 * l) / 451;
        n = (h + l - 7 * m + 114) / 31;
        p = (h + l - 7 * m + 114) % 31;
        month = n;
        day = p + 1;

    } else {
                
        /* calculate date of easter for a year in the Julian calendar   */
                
        a = year % 4;
        b = year % 7;
        c = year % 19;
        d = (19 * c + 15) % 30;
        e = (2 * a + 4 * b - d + 34) % 7;
        f = (d + e + 114) / 31;
        g = (d + e + 114) % 31;
        month = f;
        day = g + 1;
                
    }
        
    return(julian_date(day, month, year));
        
}

/**/

int day_of_week(DATE jdate) {
                                                                
    /* calculate the day of the week for the specified julian day number*/
    /* 1 = Sunday, 2 = Monday, ..., 7 = Saturday                        */
        
    return((int)(jdate + 2) % 7 + 1);
        
}

/**/

int day_of_year(DATE jdate) {
/*
 * Return the day of the year (1 - 365/6)
 */
   
   int day;
   int month;
   int year;
   DATE jan1date; 
   DATE tdate;
   
   calendar_date(jdate, &day, &month, &year);
   jan1date = julian_date(1, 1, year);
   tdate = julian_date(day, month, year);
   
   return(tdate - jan1date);
   
}

/**/

int week_of_year(DATE jdate) {
/*
 * Return the week of the year (1 - 52)
 */
   
   int day;
   int month;
   int year;
   int dn;
   int j1n;
   int wn;
   DATE jan1date;
   
   calendar_date(jdate, &day, &month, &year);
   jan1date = julian_date(1, 1, year);
   
   dn = day_of_year(jdate);
   dn += (j1n = (int)((jan1date % 7L) - 1));
   wn = (dn / 7) + 1;
   
   return(wn);
   
}

/**/

int month_of_year(DATE jdate) {
/*
 * Return the month of the year.
 */
   
   int day;
   int month;
   int year;
   
   calendar_date(jdate, &day, &month, &year);
   
   return(month);
   
}
   
/**/

int isleap(DATE jdate) {
   
   int day;
   int month;
   int year;
   
   calendar_date(jdate, &day, &month, &year);
   
   return((year % 400 == 0) || ((year % 4 == 0) && (year % 100 != 0)));
   
}

/**/

DATE today(void) {
   
   time_t tnow;
   struct tm *tmnow;
   
   time(&tnow);
   tmnow = localtime(&tnow);
   
   return(julian_date(tmnow->tm_mday, tmnow->tm_mon + 1, tmnow->tm_year + 1900));
   
}

/**/

char *moon_phase(DATE jdate) {
/*
 * Return the phase of the moon. (0 -> 7, 0 = new, 4 = full)
 */
   
   char *text[8] = {"new", "waxing cresent", "first quarter", 
                        "waxing gibbons", "full", "waning gibbons",
                        "third quarter", "waning crescent"};
   unsigned long date;
   
   date = jdate;
   date *= 9693L;
   date /= 35780L;
   date -= 4L;
   date %= 8L;
   
   return(text[date]);
   
}
   
/**/

static int leapday(DATE jdate) {
   
   int day;
   int month;
   int year;

   calendar_date(jdate, &day, &month, &year);
   
   return(month == 2 && isleap(jdate));
   
}

/**/

int valid_date(DATE jdate) {
/*
 * Return wither this is a valid date.
 */
   
   int day;
   int month;
   int year;
   int days[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
   
   calendar_date(jdate, &day, &month, &year);
   
   if (((month > 0) && (month < 13)) &&
       ((day > 0) && (day < (days[month - 1] + leapday(jdate))))) { 
                  
       return(TRUE);
          
   }
                          
  return(FALSE);
                          
}
   
/**/

DATE add_days(DATE jdate, int days) {
/*
 * Add the number of days to jdate.
 */
   
   return(jdate + days);
   
}

/**/

DATE sub_days(DATE jdate, int days) {
/*
 * Subtract the number of days from jdate.
 */
   
   return(jdate - days);
   
}

