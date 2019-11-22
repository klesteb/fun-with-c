
#include <stdio.h>
#include <stdlib.h>
#include "que_util.h"

extern int wordwrap(char *, int, queue *);

int main(int argc, char **argv) {

    queue results;
    char *line = NULL;
    char *value = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Integer aliquam eget justo ut accumsan.\n Curabitur commodo velit eu ipsum venenatis, vitae bibendum leo convallis. Vestibulum ac blandit turpis. Praesent interdum urna sapien, sit amet vestibulum enim euismod non. Fusce hendrerit ultricies interdum. Morbi dictum mauris at elit sollicitudin, a faucibus odio fringilla. Aliquam feugiat sem quis quam accumsan, ut hendrerit tortor gravida. Nam gravida ipsum vel dictum vehicula. Aliquam condimentum erat et massa auctor commodo. Aliquam vel neque et nulla luctus interdum. Donec ac nisl dapibus, ullamcorper quam vehicula, congue orci. Aliquam ac egestas nisi. Morbi non arcu in lorem dictum pharetra. Nam purus orci, egestas bibendum lacinia et, ultricies porttitor sapien. Pellentesque blandit, ante nec placerat eleifend, nisi sapien fringilla augue, ut tempus nisl libero a ligula. Ut libero sapien, semper eget mauris sed, aliquet tristique lacus.";

    que_init(&results);

    if ((wordwrap(value, 48, &results)) == 0) {

        printf("\nresults\n\n");

        while ((line = que_pop_head(&results))) {
            
            printf("%s\n", line);
            free(line);
            
        }

        printf("\n");

    } else {

        printf("unable to wrap the text\n");

    }

    return 0;

}

