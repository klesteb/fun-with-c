
/*---------------------------------------------------------------------------*/
/*                Copyright (c) 2019 by Kevin L. Esteb                       */
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
#include <time.h>
#include <log4c.h>

#include "logger_priv.h"

static const char *logstash_format(
    const log4c_layout_t        *layout,
    const log4c_logging_event_t *event)
{

    /* 
     * a basic logstash packet looks something like this 
     *                                             
     * {
     *  "@version":1,
     *  "@timestamp":"2019-09-27T15:22:59.838+0000",
     *  "@message":"This is the complete message.",
     * }
     * 
     * Where the @timestamp, @message, and @version are the only required
     * fields. This will store nicely into a Elastic Search datastore. But
     * it is kind of useless. So you need to add stuff to make it useful.
     * 
     */

    char timezone[8];
    char timestamp[30];
    time_t ltime = event->evt_timestamp.tv_sec;
    struct tm *tm = localtime(&ltime);
    char *format = "{\"@version\":1,\"@timestamp\":\"%s\",\"@message\":\"%s\",\"category\":\"%s\",\"priority\":\"%s\",\"facility\":\"%s\",\"hostname\":\"%s\",\"process\":\"%s\",\"pid\":%d,\"lineno\":%d,\"filename\":\"%s\",\"function\":\"%s\"}";

    strftime(timezone, sizeof(timezone), "%z", tm);
    snprintf(timestamp, sizeof(timestamp),  
             "%04d-%02d-%02dT%02d:%02d:%02d.%03ld%s",
             tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday,
             tm->tm_hour, tm->tm_min, tm->tm_sec,
             event->evt_timestamp.tv_usec / 1000, timezone
    );
    user_local_info_t *user = (user_local_info_t*)event->evt_loc->loc_data;;

    snprintf(event->evt_buffer.buf_data, event->evt_buffer.buf_size,
             format, timestamp, 
             event->evt_msg, event->evt_category, 
             log4c_priority_to_string(event->evt_priority),
             user->facility, user->hostname, user->process, user->pid,
             event->evt_loc->loc_line, event->evt_loc->loc_file,
             event->evt_loc->loc_function
    );

    return event->evt_buffer.buf_data;

}

static const char *console_format(
    const log4c_layout_t        *layout,
    const log4c_logging_event_t *event)
{

    /* 
     * console format
     * 
     * <category> - <message>
     */

    char *format = "%-5s - %s";

    snprintf(event->evt_buffer.buf_data, event->evt_buffer.buf_size,
             format, 
             log4c_priority_to_string(event->evt_priority),
             event->evt_msg
    );

    return event->evt_buffer.buf_data;

}

static const char *batch_format(
    const log4c_layout_t        *layout,
    const log4c_logging_event_t *event)
{

    /* 
     * batch format
     * 
     * [<timestamp>] <priority> -  <message>
     */

    char timestamp[30];
    time_t ltime = event->evt_timestamp.tv_sec;
    struct tm *tm = localtime(&ltime);
    char *format = "[%s] %-5s - %s";

    snprintf(timestamp, sizeof(timestamp),  
             "%04d-%02d-%02d %02d:%02d:%02d",
             tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday,
             tm->tm_hour, tm->tm_min, tm->tm_sec,
             event->evt_timestamp.tv_usec / 1000
    );

    snprintf(event->evt_buffer.buf_data, event->evt_buffer.buf_size,
             format, timestamp, 
             log4c_priority_to_string(event->evt_priority),
             event->evt_msg
    );

    return event->evt_buffer.buf_data;

}

/* layouts */

static const log4c_layout_type_t log4c_layout_type_logstash = {
    "logstash",
    logstash_format,
};

static const log4c_layout_type_t log4c_layout_type_console = {
    "console",
    console_format,
};

static const log4c_layout_type_t log4c_layout_type_batch = {
    "batch",
    batch_format,
};

static const log4c_layout_type_t * const layout_types[] = {
    &log4c_layout_type_logstash,
    &log4c_layout_type_console,
    &log4c_layout_type_batch,
};

static int nlayout_types =
	(int)(sizeof(layout_types) / sizeof(layout_types[0]));

/*****************************/
/*
 * Here provide an init routine for this lib 
 *
******************************/

int log4c_extensions_init(void) {

    int i = 0;
    int rc = 0; 

    for (i = 0; i < nlayout_types; i++) {

        log4c_layout_type_set(layout_types[i]);

    }

    return rc;

}

