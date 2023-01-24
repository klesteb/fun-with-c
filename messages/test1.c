
#include <stdio.h>

#include "message.h"

int main(int argc, char **argv) {

    int stat = OK;
    char msgbuf[1024];
    message_t *msgs = NULL;
    messages_t messages[] = {
        { 11, "testing 2" },
        { 12, "TESTING 12"  }
    };

    if ((msgs = message_create(messages, sizeof(messages)))) {

        stat = message_add(msgs, 10, "testing 1");
        if (stat != OK) {

            printf("Unable to add message\n");

        }

        stat = message_get(msgs, 10, msgbuf, 1023);
        if (stat == OK) {

            printf("Message: %s\n", msgbuf);

        } else {

            printf("Unable to get message\n");

        }

        stat = message_get(msgs, M_STARTUP, msgbuf, 1023);
        if (stat == OK) {

            printf("%s\n", msgbuf);

        } else {

            printf("Unable to get message\n");

        }

        stat = message_set(msgs, 10, "testing again");
        if (stat != OK) {

            printf("Unable to set message\n");

        }

        stat = message_get(msgs, 10, msgbuf, 1023);
        if (stat == OK) {

            printf("Message: %s\n", msgbuf);

        } else {

            printf("Unable to get message\n");

        }

        stat = message_remove(msgs, 10);
        if (stat != OK) {

            printf("Unable to remove message\n");

        }

        stat = message_get(msgs, 10, msgbuf, 1023);
        if (stat == OK) {

            printf("Message: %s\n", msgbuf);

        } else {

            printf("Unable to get message\n");

        }

        stat = message_get(msgs, 11, msgbuf, 1023);
        if (stat == OK) {

            printf("Message: %s\n", msgbuf);

        } else {

            printf("Unable to get message\n");

        }

        message_destroy(msgs);

    } else {

        printf("Unable to create object\n");

    }

    return 0;

}

