
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "str_util.h"
#include "cfg_util.h"

extern char *xstrtok(char *, char *);
extern void *xmalloc(int);
extern int   stricmp(const char *, const char *);
extern char *strupr(char *);


int main(int argc, char **argv) {

    int stat;
    char *holder;
    char *quotes = "\"";
    char junk[256];
    char fileops[256];
    char accounts[256];
    char logfile[1024];
    char ip_port[10];
    char welcome[256];
    char *filename = "test.ini";
    char root[256];
    int acct_cnt;
    config_t configs;


    printf("\nTesting with %s\n\n", filename);
    
    if ((stat = cfg_open(filename, &configs)) == 0) {

        cfg_get(&configs, "settings", "port", "115", ip_port);
        cfg_get(&configs, "settings", "welcome", "A SFTP Server", welcome);
        cfg_get(&configs, "settings", "root", "", junk);
        cfg_get(&configs, "settings", "fileops", "store,retrieve,chdir,delete,rename,list", fileops);
        cfg_get(&configs, "settings", "accounts", "1", accounts);
        cfg_get(&configs, "settings", "logfile", "server.log", logfile);

        acct_cnt = atoi(accounts);
        strupr(fileops);
        strcpy(root, junk);

        holder = str_destring(welcome, strlen(welcome), quotes);
        strcpy(welcome, holder);

        printf("port = %s\n", ip_port);
        printf("welcome = %s\n", welcome);
        printf("root = %s\n", root);
        printf("accounts = %d\n", acct_cnt);
        printf("logfile = %s\n", logfile);
        printf("\nfileops found\n\n");
        
        for (holder = xstrtok(fileops, ",");
             holder != NULL;
             holder = xstrtok(NULL, ",")) {
        
            if (stricmp("STORE", holder) == 0) {

                printf("\"store\" was found\n");

            } else if (stricmp("RETRIEVE", holder) == 0) {

                printf("\"retrieve\" was found\n");

            } else if (stricmp("CHDIR", holder) == 0) {

                printf("\"chdir\" was found\n");

            } else if (stricmp("DELETE", holder) == 0) {

                printf("\"delete\" was found\n");

            } else if (stricmp("RENAME", holder) == 0) {

                printf("\"rename\" was found\n");

            } else if (stricmp("LIST", holder) == 0) {

                printf("\"list\" was found\n");

            }

        }

		printf("\n");
		
    }

    exit(EXIT_SUCCESS);
    
}

