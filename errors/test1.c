
#include <stdio.h>
#include <errno.h>
#include "errors.h"

int main(void) {

    char text[1024];
    char message[1024];
    errors_t *errors = errors_create();

    errors_get_message(errors, E_INVPARM, message, 1023);
    printf("%s\n", message);

    errors_get_text(errors, EAGAIN, text, 1023);
    errors_get_message(errors, EAGAIN, message, 1023);
    printf("%s: %s\n", text, message);

    errors_destroy(errors);

    return 0;

}

