
#include <stdio.h>
#include "include/when.h"

int main(int argc, char **argv) {

    when_error
        
       cause_error(10)
    
    use
    
       printf("Error: %d, line: %d, file: %s, function: %s\n",
              _er_trace.errnum, _er_trace.lineno, _er_trace.filename,
              _er_trace.function);
       clear_error();
    
    end_when

    return 0;

}

