
#include <stdio.h>
#include "template.h"

int main(int argc, char **argv) {

    template_t *temp = NULL;

    if ((temp = template_create(NULL))) {

        printf("version = %s\n", template_version(temp));
        template_destroy(temp);

    }

    return 0;

}

