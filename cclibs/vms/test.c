
#include <stdio.h>
#include <stdlib.h>
#include "vms.h"

SUBPROC sub1;
SUBPROC sub2;

int main(void) {

    queue sub1_list;
    queue sub2_list;

    que_init(&sub1_list);
    que_init(&sub2_list);

    vms_create_subprocess(&sub1);
    vms_create_subprocess(&sub2);

    vms_execute_command(&sub1, "reply/term=tna14: \"Hello from 1\"", &sub1_list);
    vms_execute_command(&sub2, "reply/term=tna14: \"Hello from 2\"", &sub2_list);
 
    sleep(5);

    vms_delete_subprocess(&sub1);

}

