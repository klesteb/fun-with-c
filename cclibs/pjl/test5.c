#include <stdio.h>
#include <pcre.h>
#include <string.h>

int main(void) {

    /* parsing ustatus detail line */

    char name[32];
    char value[32];
    char count[32];
    char type[32];
    const char *error;
    int rc = 0;
    int stat = 0;
    int errorOffset = 0; 
    int ovector[30];
    char *buffer = "DEVICE=OFF [3 ENUMERATED]";
    char *pattern = "^(\\w+)=(\\w+)\\s+\\[(\\d+)\\s+(\\w+)\\]"; 
    pcre *re = NULL;
    
    if ((re = pcre_compile(pattern, 0, &error, &errorOffset, NULL)) == NULL) {

        printf("error: %s\n", error);
        printf("offset: %d\n", errorOffset);

    }

    if ((stat = pcre_exec(re, NULL, buffer, strlen(buffer), 0, 0, ovector, 30)) < 0) {
        
        printf("no matches: %d\n", stat);
        
    }
    
    if ((rc = pcre_copy_substring(buffer, ovector, stat, 1, name, 31)) < 0) { 
        
        printf("unable to find name: %d\n", rc);
        
    }
        
    if ((rc = pcre_copy_substring(buffer, ovector, stat, 2, value, 31)) < 0) { 
        
        printf("unable to find value: %d\n", rc);

    }

    if ((rc = pcre_copy_substring(buffer, ovector, stat, 3, count, 31)) < 0) { 
        
        printf("unable to find count: %d\n", rc);

    }

    if ((rc = pcre_copy_substring(buffer, ovector, stat, 4, type, 31)) < 0){ 
        
        printf("unable to find type: %d\n", rc);

    }

    printf("name = %s\n", name);
    printf("value = %s\n", value);
    printf("count = %s\n", count);
    printf("type = %s\n", type);
        
    return 0;

}

