
#include <stdio.h>
#include <stdlib.h>
#include "deque.h"

void vms_mail(char *, char *, char *, char *, queue *);

int main(void) {

    queue qbody;
    char *from = "backup";
    char *to = "@wss$oper:saver.dis";
    char *cc = "";
    char *subject = "Testing";
    char *message = "This is a test...";
    char *junk = NULL;

    que_init(&qbody);
    que_push_tail(&qbody, message);

    vms_mail(from, to, cc, subject, &qbody);

    junk = que_pop_tail(&qbody);

}

