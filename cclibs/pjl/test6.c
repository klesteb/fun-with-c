#include <stdio.h>
#include <pcre.h>
#include <string.h>

int main(void) {

    /* parsing config */

    char name[32];
    char value[32];
    char items[32];
    char type[32];
    const char *error;
    int rc = 0;
    int count = 0;
    int offset = 0; 
    int ovector[30];
    char *buff1 = "LANGUAGES [3 ENUMERATED]";
    char *buff2 = "MEMORY=100663296";
    char *pat1 = "(\\w+)\\s+\\[(\\d+)\\s+(\\w+)\\]"; 
    char *pat2 = "^(\\w+)=(\\d+)";
    pcre *re1 = NULL;
    pcre *re2 = NULL;

    if ((re1 = pcre_compile(pat1, 0, &error, &offset, NULL)) == NULL) {

        printf("error: %s\n", error);
        printf("offset: %d\n", offset);

    }

    if ((re2 = pcre_compile(pat2, 0, &error, &offset, NULL)) == NULL) {

        printf("error: %s\n", error);
        printf("offset: %d\n", offset);

    }

    printf("Parsing: \"%s\"\n", buff1);
    
    if ((count = pcre_exec(re1, NULL, buff1, strlen(buff1), 0, 0, ovector, 30)) > 0) {

        if ((rc = pcre_copy_substring(buff1, ovector, count, 1, name, 31)) < 0) {

            printf("error finding name: %d\n", rc);

        }

        if ((rc = pcre_copy_substring(buff1, ovector, count, 2, items, 31)) < 0) {

            printf("error finding items: %d\n", rc);

        }

        if ((rc = pcre_copy_substring(buff1, ovector, count, 3, type, 31)) < 0) {

            printf("error finding type: %d\n", rc);

        }

        printf("name = %s\n", name);
        printf("items = %s\n", items);
        printf("type = %s\n", type);

    }

    printf("Parsing: \"%s\"\n", buff2);

    if ((count = pcre_exec(re2, NULL, buff2, strlen(buff2), 0, 0, ovector, 30)) > 0) {

        if ((rc = pcre_copy_substring(buff2, ovector, count, 1, name, 31)) < 0) {

            printf("error finding name: %d\n", rc);

        }

        if ((rc = pcre_copy_substring(buff2, ovector, count, 2, value, 31)) < 00) {

            printf("error finding value: %d\n", rc);

        }

        printf("name = %s\n", name);
        printf("value = %s\n", value);

    }

    return 0;

}

