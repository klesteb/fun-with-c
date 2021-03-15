
#include <stdio.h>
#include <errno.h>

#include "node.h"
#include "when.h"
#include "finds.h"
#include "files.h"
#include "errors.h"
#include "tracer.h"
#include "que_util.h"
#include "misc/misc.h"

node_t *nodes;
tracer_t *dump;
errors_t *errs;

int display(node_base_t *temp) {

    printf("---------------------------------\n");
    printf("node    : %ld\n", temp->nodenum);
    printf("status  : %d\n", temp->status);
    printf("errors  : %d\n", temp->errors);
    printf("action  : %d\n", temp->action);
    printf("user    : %d\n", temp->useron);
    printf("msgnum  : %ld\n", temp->msgnum);
    printf("misc    : %d\n", temp->misc);
    printf("aux     : %d\n", temp->aux);
    printf("extaux  : %ld\n", temp->extaux);
    printf("revision: %d\n", temp->revision);

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

        nodes = node_create(path, 32, retries, timeout, dump);
        check_creation(nodes);

        exit_when;

    } use {

        stat = ERR;
        capture_trace(dump);
        clear_error();

    } end_when;

    return stat;

}

void cleanup(void) {

    node_destroy(nodes);
    tracer_destroy(dump);
    errors_destroy(errs);

}

int main(int argc, char **argv) {

    int stat = OK;
    queue results;
    node_base_t temp;
    node_search_t *result = NULL;

    when_error_in {

        stat = que_init(&results);
        check_status(stat, OK, E_INVOPS);

        stat = setup();
        check_status(stat, OK, E_INVOPS);

        stat = node_open(nodes);
        check_return(stat, nodes);

        stat = node_search(nodes, NULL, 0, find_nodes_all, &results);
        check_return(stat, nodes);

        while (result = que_pop_head(&results)) {

            stat = node_get(nodes, result->index, &temp);
            check_return(stat, nodes);

            display(&temp);

        }

        stat = node_close(nodes);
        check_return(stat, nodes);

        exit_when;

    } use {

        capture_trace(dump);
        tracer_dump(dump, dump_trace);
        clear_error();

    } end_when;

    cleanup();

    return 0;

}

