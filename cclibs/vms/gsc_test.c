
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "vms.h"
#include "gsc.h"

struct data {
    char string[80][80];
};

GblSec sec;

int main(void) {

    int x;
    int stat;
    struct data *data;
    

    stat = gsc_open("test.dat", "testing", sizeof(struct data), &sec);
    data = gsc_associate(&sec);

    stat = gsc_lock(&sec);

    for (x = 0; x < 80; x++) {

        strcpy(data->string[x], "Testing...");

    }

    stat = gsc_flush(&sec);
    stat = gsc_unlock(&sec);

/*    gsc_close(&sec); */

}

