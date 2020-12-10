
#include <stdio.h>
#include <errno.h>

#include "jam.h"
#include "qwk.h"
#include "when.h"
#include "errors.h"
#include "tracer.h"
#include "misc/misc.h"

jam_t *jam;
qwk_t *qwk;
tracer_t *dump;
errors_t *errs;

int compare(ulong selector, jam_message_t *message) {

    return TRUE;

}

int dump_trace(char *buffer) {

    fprintf(stderr, "%s\n", buffer);

    return OK;

}

int setup(void) {

    int base = 1;
    int stat = OK;
    int timeout = 1;
    char *path = ".";
    char retries = 30;
    char *name = "001";


    when_error_in {

        errs = errors_create();
        check_creation(errs);

        dump = tracer_create(errs);
        check_creation(dump);

        jam = jam_create(path, name, retries, timeout, base, dump);
        check_creation(jam);

        qwk = qwk_create(path, retries, timeout, FALSE, dump);
        check_creation(qwk);

        exit_when;

    } use {

        stat = ERR;
        capture_trace(dump);
        clear_error();

    } end_when;

    return stat;

}

void cleanup(void) {

    jam_destroy(jam);
    qwk_destroy(qwk);
    tracer_destroy(dump);
    errors_destroy(errs);

}

int build_control(qwk_control_t *control) {

    int stat = OK;
    qwk_area_t *area = NULL;

    when_error_in {

        stat = que_init(&control->areas);
        check_status(stat, QUE_OK, E_INVOPS);

        strcpy((*control).bbs_name, "test bbs");
        strcpy((*control).city, "everett");
        strcpy((*control).state, "wa");
        strcpy((*control).phone, "425-555-1212");
        strcpy((*control).sysop, "kevin esteb");
        strcpy((*control).serial_num, "123456");
        strcpy((*control).bbs_id, "testing");
        strcpy((*control).username, "kevin esteb");
        strcpy((*control).hello_file, "");
        strcpy((*control).news_file, "");
        strcpy((*control).goodbye_file, "");

        (*control).num_areas = 2;
        (*control).date_time = time(NULL);

        int x;
        for (x = 1; x <= control->num_areas; x++) {

            errno = 0;
            area = calloc(1, sizeof(qwk_area_t));
            if (area == NULL) cause_error(errno);

            area->area = x;
            sprintf(area->name, "area %d", x);

            stat = que_push_tail(&control->areas, area);
            check_status(stat, QUE_OK, E_NOQUEUE);

        }

        exit_when;

    } use {

        stat = ERR;
        capture_trace(dump);
        clear_error();

    } end_when;

    return stat;

}

int main(int argc, char **argv) {

	queue fields;
    queue results;
    int stat = OK;
    qwk_ndx_t *ndx;
    int msgnum = 0;
    int sequence = 0;
    ulong record = 0;
    ssize_t count = 0;
    char *text = NULL;
    ulong user_crc = 0;
    int conference = 1;
    qwk_header_t *header;
    char *sysop = "sysop";
    qwk_control_t control;
    jam_message_t *message;
    jam_field_t *field = NULL;
    jam_search_t *search = NULL;
    char *notice = "testing testing";
    char *net_tag = " ";
    char *password = "            ";

    when_error_in {

        stat = setup();
        check_status(stat, OK, E_INVOPS);

        stat = build_control(&control);
        check_status(stat, OK, E_INVOPS);

        stat = que_init(&results);
        check_status(stat, QUE_OK, E_INVOPS);

        stat = jam_crc32(jam, (uchar *)sysop, strlen(sysop), &user_crc);
        check_return(stat, jam);

        stat = qwk_open(qwk);
        check_return(stat, qwk);

        stat = jam_open(jam);
        check_return(stat, jam);

        stat = qwk_put_control(qwk, &control);
        check_return(stat, qwk);

        stat = qwk_put_notice(qwk, notice);
        check_return(stat, qwk);

        stat = qwk_open_ndx(qwk, "001");
        check_return(stat, qwk);

        stat = jam_search_messages(jam, user_crc, TRUE, compare, &results);
        check_return(stat, jam);

        printf("username: %s\n", sysop);
        printf("crc     : %ld\n", user_crc);
        printf("messages: %d\n\n", que_size(&results));

        while ((search = que_pop_head(&results))) {

            sequence++;

            que_init(&fields);
            check_status(stat, QUE_OK, E_INVOPS);

            stat = jam_new_message(jam, &message);
            check_return(stat, jam);

            stat = qwk_new_header(qwk, &header);
            check_return(stat, qwk);

            stat = jam_get_message(jam, search->msgnum, message, &fields, &text);
            check_return(stat, jam);

            msgnum = search->msgnum;
            printf("msgnum: %d\n", msgnum + 1);

            while ((field = que_pop_head(&fields))) {

                if (field->lo_id == JAMSFLD_SENDERNAME) {

                    strncpy(header->from, (char *)field->buffer, 24);

                }

                if (field->lo_id == JAMSFLD_RECVRNAME) {

                    strncpy(header->to, (char *)field->buffer, 24);

                }

                if (field->lo_id == JAMSFLD_SUBJECT) {

                    strncpy(header->subject, (char *)field->buffer, 24);

                }

                stat = jam_free_field(jam, field);
                check_return(stat, jam);

            }

            strncpy(header->password, password, 12);

            header->status = QWK_PUB_UNREAD;
            header->number = search->msgnum;
            header->date_time = message->date_written;
            header->reply = 0;
            header->records = QWK_REC_CNT(strlen(text));
            header->alive = 0xe1;
            header->conference = conference;
            header->seq_number = sequence;
            strncpy(header->net_tag, net_tag, 1);

            stat = qwk_put_message(qwk, header, text, &record);
            check_return(stat, qwk);
            printf("ndx record: %ld\n", record);

            stat = qwk_new_ndx(qwk, record, conference, &ndx);
            check_return(stat, qwk);

            stat = qwk_put_ndx(qwk, ndx, &count);
            check_return(stat, qwk);

            free(message);
            free(header);
            free(text);
            free(ndx);
            
        }

        stat = qwk_close(qwk);
        check_return(stat, qwk);

        stat = jam_close(jam);
        check_return(stat, jam);

        exit_when;

    } use {

        capture_trace(dump);
        tracer_dump(dump, dump_trace);
        clear_error();

    } end_when;

    cleanup();

    return 0;

}

