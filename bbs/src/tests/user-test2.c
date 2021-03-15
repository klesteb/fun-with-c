
#include <stdio.h>
#include <errno.h>

#include "user.h"
#include "when.h"
#include "finds.h"
#include "files.h"
#include "errors.h"
#include "tracer.h"
#include "misc/misc.h"

user_t *users;
tracer_t *dump;
errors_t *errs;

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

        stat = user_search(users, NULL, 0, find_users_all, &results);
        check_return(stat, users);

        printf("\nfound %d users\n", que_size(&results));

        while ((result = que_pop_head(&results))) {

            stat = user_get(users, result->index, &ondisk);
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

