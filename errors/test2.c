
#include <stdio.h>
#include <errno.h>
#include "errors.h"

int main(void) {

    char message[1024];
    errors_t *errors = errors_create();

    errors_get_message(errors, E_INVPARM, message, 1023);
    printf("%s\n", message);

    errors_get_message(errors, EAGAIN, message, 1023);
    printf("%s\n", message);

    memset(message, '\0', 1024);
    errors_remove(errors, EAGAIN);
    errors_get_message(errors, EAGAIN, message, 1023);
    printf("%s\n", message);

    errors_add(errors, EAGAIN, "EAGAIN", strerror(EAGAIN));
    errors_get_message(errors, EAGAIN, message, 1023);
    printf("%s\n", message);

    errors_destroy(errors);

    return 0;

}

