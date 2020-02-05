
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tcp_util.h"

int main (int argc, char **argv) {

    char  buffer[128] ;
    int  i ;
    char *servername = "9100@berlin";
    TcpEndpoint  server ;

    tcp_call("9100@berlin", 0, 0, &server);

    for (i = 0 ;  i < 100 ;  i++) {

        sprintf(buffer, "Line #%d...\r\n", i) ;
        tcp_write(server, -1.0, strlen(buffer), buffer, NULL) ;

    }

    sprintf(buffer, "\f");
    tcp_write(server, -1.0, strlen(buffer), buffer, NULL);

    tcp_destroy(server);

    exit(EXIT_SUCCESS);

}

