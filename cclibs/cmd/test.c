
#include <stdio.h>
#include <stdlib.h>

#include "cmd_util.h"


int test(void *data) {

    int index;
    char *argument = NULL;

    printf("This is function test\n");

    while ((index = cmd_get(&argument)) != CMD_END) {

        switch (index) {
            case 1:
                printf("The -a qualifier was specified\n");
                break;

            case 2:
                printf("The -file qualifier was specified\n");
                printf("The value = %s\n", argument);
                break;

            case CMD_NOOPT:
                printf("The parameter value = %s\n", argument);
                break;

            case CMD_ERROR:
                printf("Error processing the qualifiers\n");

        }

    }

    return(0);

}

int quit(void *data) {

    printf("This is function quit\n");

    cmd_destroy();
    exit(EXIT_SUCCESS);

}

int main(void) {

    int stat;
    CommandTable table[3];

    SET_TABLE(table[0], "TEST", "a{file:}", test);
    SET_TABLE(table[1], "QUIT", NULL, quit);
    SET_TABLE(table[2], NULL, NULL, NULL);

    if ((stat = cmd_create(table, NULL, "TEST> ")) == 0) {

        while ((stat = cmd_input()) == 0) {

            stat = cmd_dispatch(NULL);

        }

    }

}

