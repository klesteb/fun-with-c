#include <stdio.h>
#include <pcre.h>
#include <string.h>

int main(void) {

    /* parsing variables */

    char name[32];
    char value[32];
    char items[32];
    char type[32];
    const char *error;
    int count = 0;
    int offset = 0; 
    int ovector[30];
    char *buffer = "PAPER=LETTER [17 ENUMERATED]";
    char *pattern = "^(\\w+)=(\\w+)\\s+\\[(\\d+)\\s+(\\w+)\\]"; 
    pcre *re = NULL;

    if ((re = pcre_compile(pattern, 0, &error, &offset, NULL)) == NULL) {

        printf("error: %s\n", error);
        printf("offset: %d\n", offset);

    }

    printf("parsing: \"%s\"\n", buffer);

    if ((count = pcre_exec(re, NULL, buffer, strlen(buffer), 0, 0, ovector, 30)) > 0) {

        pcre_copy_substring(buffer, ovector, count, 1, name, 31);
        pcre_copy_substring(buffer, ovector, count, 2, value, 31);
        pcre_copy_substring(buffer, ovector, count, 3, items, 31); 
        pcre_copy_substring(buffer, ovector, count, 4, type, 31);

    }

    printf("name  = %s\n", name);
    printf("value = %s\n", value);
    printf("items = %s\n", items);
    printf("type  = %s\n", type);

    return 0;

}

