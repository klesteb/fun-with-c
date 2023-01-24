
#include <stdio.h>
#include <errno.h>

#include "rms/files.h"
#include "include/when.h"
#include "errors/errors.h"
#include "tracer/tracer.h"
#include "cclibs/misc/misc.h"

#include "bbs/src/user/user.h"
#include "bbs/src/finds/finds.h"

rms_t *users;
tracer_t *dump;
errors_t *errs;

int capture(rms_t *self, void *data, queue *results) {

    int stat = OK;
    user_base_t *ondisk = NULL;
    user_search_t *result = NULL;

    when_error_in {

        ondisk = (user_base_t *)data;

        errno = 0;
        result = calloc(1, sizeof(user_search_t));
        if (result == NULL) cause_error(errno);

        strncpy(result->username, ondisk->username, LEN_NAME);
        result->record = self->record;
        result->profile = ondisk->profile;

        stat = que_push_head(results, result);
        check_status(stat, QUE_OK, E_NOQUEUE);

        exit_when;

    } use {

        stat = ERR;
        capture_trace(dump);
        clear_error();

    } end_when;
    
    return stat;

}

int display(user_base_t *user) {

    printf("---------------------------\n");
    printf("eternal : %ld\n", user->eternal);
    printf("username: %s\n", user->username);

    return OK;

}

int dump_trace(char *buffer) {

    fprintf(stderr, "%s\n", buffer);

    return OK;

}

int setup(void) {

    int stat = OK;
    int timeout = 1;
    int retries = 30;
    char *path = "../../data/";

    when_error_in {

        errs = errors_create();
        check_creation(errs);

        dump = tracer_create(errs);
        check_creation(dump);

        users = user_create(path, 256, retries, timeout, dump);
        check_creation(users);

        exit_when;

    } use {

        stat = ERR;
        capture_trace(dump);
        clear_error();

    } end_when;

    return stat;

}

void cleanup(void) {

    user_destroy(users);
    tracer_destroy(dump);
    errors_destroy(errs);

}

int main(int argc, char **argv) {

    int stat = OK;
    queue results;
    user_base_t ondisk;
    user_search_t *result = NULL;

    when_error_in {

        stat = que_init(&results);
        check_status(stat, QUE_OK, E_INVOPS);

        stat = setup();
        check_status(stat, OK, E_INVOPS);

        stat = user_open(users);
        check_return(stat, users);

        stat = user_search(users, NULL, 0, find_users_all, capture, &results);
        check_return(stat, users);

        printf("\nfound %d users\n", que_size(&results));

        while ((result = que_pop_head(&results))) {

            stat = user_get(users, result->record, &ondisk);
            check_return(stat, users);

            display(&ondisk);
            free(result);

        }

        stat = user_close(users);
        check_return(stat, users);

        exit_when;

    } use {

        capture_trace(dump);
        tracer_dump(dump, dump_trace);
        clear_error();

    } end_when;

    cleanup();

    return 0;

}

