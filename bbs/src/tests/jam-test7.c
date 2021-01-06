
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

int compare(ulong selector, jam_message_t *message) {

    return TRUE;

}

int dump_trace(char *buffer) {

    fprintf(stderr, "%s\n", buffer);

    return OK;

}

int main(int argc, char **argv) {

    int base = 1;
    queue lines;
    queue fields;
    queue results;
    int stat = OK;
    int timeout = 1;
    ulong msgnum = 0;
    char *path = ".";
    char retries = 30;
    char *text = NULL;
    char *line = NULL;
    char *name = "001";
    ulong user_crc = 0;
    char *sysop = "sysop";
    jam_lastread_t lastread;
    jam_field_t *field = NULL;
    jam_search_t *search = NULL;
    jam_message_t *message = NULL;

    when_error_in {

        errors = errors_create();
        check_creation(errors);

        dump = tracer_create(errors);
        check_creation(dump);

        jam = jam_create(path, name, retries, timeout, base, dump);
        check_creation(jam);

        stat = jam_open(jam);
        check_return(stat, jam);

        stat = jam_crc32(jam, (uchar *)sysop, strlen(sysop), &user_crc);
        check_return(stat, jam);

        stat = jam_get_lastread(jam, user_crc, &lastread);
        check_return(stat, jam);

        /* retrieve the messages */

        printf("username: %s\n", sysop);
        printf("crc     : %ld\n", user_crc);

        stat = que_init(&results);
        check_status(stat, QUE_OK, E_INVOPS);

        stat = jam_search_messages(jam, user_crc, TRUE, compare, &results);
        check_return(stat, jam);

        printf("messages: %d\n\n", que_size(&results));

        while ((search = que_pop_head(&results))) {

            que_init(&fields);
            check_status(stat, QUE_OK, E_INVOPS);

            que_init(&lines);
            check_status(stat, QUE_OK, E_INVOPS);

            stat = jam_new_message(jam, &message);
            check_return(stat, jam);

            stat = jam_get_message(jam, search->msgnum, message, &fields, &text);
            check_return(stat, jam);

            msgnum = search->msgnum;
            printf("msgnum: %d\n", msgnum + 1);

            while ((field = que_pop_head(&fields))) {

                if (field->lo_id == JAMSFLD_SENDERNAME) {

                    printf("From: %s\n", (char *)field->buffer);

                }

                if (field->lo_id == JAMSFLD_RECVRNAME) {

                    printf("To: %s\n", (char *)field->buffer);

                }

                if (field->lo_id == JAMSFLD_SUBJECT) {

                    printf("Subject: %s\n", (char *)field->buffer);

                }

                stat = jam_free_field(jam, field);
                check_return(stat, jam);

            }

            printf("Date: %ld\n", message->date_written);
            printf("Size: %d\n", strlen(text));
            printf("\n");

            wordwrap(text, 60, &lines);

            while ((line = que_pop_head(&lines))) {

                printf("%s\n", line);
                free(line);

            }

            printf("\n");

            free(text);
            free(search);
            free(message);

        }

        /* update the lastread record */

        lastread.last_read_msg = msgnum;
        lastread.high_read_msg = msgnum;
        
        stat = jam_put_lastread(jam, user_crc, &lastread);
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

