
#include <stdio.h>
#include <errno.h>
#include <time.h>
#include <string.h>

#include "jam.h"
#include "when.h"
#include "errors.h"
#include "tracer.h"
#include "misc/misc.h"

jam_t *jam;
tracer_t *dump;
errors_t *errors;

int dump_trace(char *buffer) {

    fprintf(stderr, "%s\n", buffer);

    return OK;

}

int main(int argc, char **argv) {

    int base = 1;
    queue fields;
    int stat = OK;
    char zone[10];
    struct tm *tm;
    int timeout = 1;
    char *path = ".";
    char retries = 30;
    char *name = "001";
    char *sysop = "sysop";
    char *to = "jane doe";
    char *subject = "testing";
    jam_field_t *field1 = NULL;
    jam_field_t *field2 = NULL;
    jam_field_t *field3 = NULL;
    jam_field_t *field4 = NULL;
    jam_message_t *message = NULL;
    char *text = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Integer aliquam eget justo ut accumsan. Curabitur commodo velit eu ipsum venenatis, vitae bibendum leo convallis. Vestibulum ac blandit turpis. Praesent interdum urna sapien, sit amet vestibulum enim euismod non. Fusce hendrerit ultricies interdum. Morbi dictum mauris at elit sollicitudin, a faucibus odio fringilla. Aliquam feugiat sem quis quam accumsan, ut hendrerit tortor gravida. Nam gravida ipsum vel dictum vehicula. Aliquam condimentum erat et massa auctor commodo. Aliquam vel neque et nulla luctus interdum. Donec ac nisl dapibus, ullamcorper quam vehicula, congue orci. Aliquam ac egestas nisi. Morbi non arcu in lorem dictum pharetra. Nam purus orci, egestas bibendum lacinia et, ultricies porttitor sapien. Pellentesque blandit, ante nec placerat eleifend, nisi sapien fringilla augue, ut tempus nisl libero a ligula. Ut libero sapien, semper eget mauris sed, aliquet tristique lacus.";

    when_error_in {

        errors = errors_create();
        check_creation(errors);

        dump = tracer_create(errors);
        check_creation(dump);

        jam = jam_create(path, name, retries, timeout, base, dump);
        check_creation(jam);

        stat = jam_open(jam);
        check_return(stat, jam);

        /* create a message */

        que_init(&fields);
        check_status(stat, QUE_OK, E_INVOPS);

        stat = jam_new_message(jam, &message);
        check_return(stat, jam);

        /* set time and the local timezone */

        message->date_written = time(NULL);

        memset(zone, '\0', 10);
        tm = localtime(&message->date_written);
        strftime(zone, 9, "%z", tm);

        stat = jam_new_field(jam, JAMSFLD_TZUTCINFO, zone, &field1);
        check_return(stat, jam);

        stat = que_push_tail(&fields, field1);
        check_status(stat, QUE_OK, E_NOQUEUE);

        stat = jam_new_field(jam, JAMSFLD_SENDERNAME, sysop, &field2);
        check_return(stat, jam);

        stat = que_push_tail(&fields, field2);
        check_status(stat, QUE_OK, E_INVOPS);

        /* message receiver (To:) */

        stat = jam_new_field(jam, JAMSFLD_RECVRNAME, to, &field3);
        check_return(stat, jam);

        stat = que_push_tail(&fields, field3);
        check_status(stat, QUE_OK, E_NOQUEUE);

        /* message subject (Subject:) */
        
        stat = jam_new_field(jam, JAMSFLD_SUBJECT, subject, &field4);
        check_return(stat, jam);

        stat = que_push_tail(&fields, field4);
        check_status(stat, QUE_OK, E_NOQUEUE);

        /* add the message to the msgbase */

        stat = jam_add_message(jam, message, &fields, text);
        check_return(stat, jam);

        stat = jam_close(jam);
        check_return(stat, jam);

        jam_destroy(jam);
        tracer_destroy(dump);
        errors_destroy(errors);

        exit_when;

    } use {

        capture_trace(dump);
        tracer_dump(dump, dump_trace);
        clear_error();

    } end_when;

    return 0;

}

