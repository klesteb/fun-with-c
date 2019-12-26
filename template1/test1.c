
#include <stdio.h>
#include "template.h"

int main(int argc, char **argv) {

    template_t *temp = NULL;

    if ((temp = template_create(NULL))) {

        template_destroy(temp);

    }

    return 0;

}

