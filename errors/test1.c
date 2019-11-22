
#include <stdio.h>
#include <errno.h>
#include "errors.h"

int main(void) {

    char *message;
    errors_t *errors = errors_create();
    
    message = errors_get_message(errors, E_OK);
    printf("%s\n", message);

    message = errors_get_message(errors, EAGAIN);
    printf("%s\n", message);

    errors_destroy(errors);

    return 0;

}

