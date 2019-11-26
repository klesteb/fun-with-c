
#include <stdio.h>
#include <errno.h>
#include "errors.h"

int main(void) {

    char *text;
    char *message;
    errors_t *errors = errors_create();
    
    message = errors_get_message(errors, E_OK);
    printf("%s\n", message);

    text = errors_get_text(errors, EAGAIN);
    message = errors_get_message(errors, EAGAIN);
    printf("%s: %s\n", text, message);

    errors_destroy(errors);

    return 0;

}

