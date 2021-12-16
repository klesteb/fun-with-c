
#include <stdio.h>

#include "objects/object.h"
#include "worker1.h"
#include "worker2.h"
#include "factory.h"

int main(void) {

    int stat = 0;
    int type1 = 0;
    int type2 = 1;
    char *result = NULL;
    factory_t *factory = NULL;
    worker_t * worker1 = NULL;
    worker_t * worker2 = NULL;
    char *data1 = "this is worker 1";
    char *data2 = "this is worker 2";

    factory = factory_create();
    worker1 = worker1_create(type1);
    worker2 = worker2_create(type2);

    stat = factory_add(factory, worker1);
    printf("added worker1, stat = %d\n", stat);

    stat = factory_add(factory, worker2);
    printf("added worker2, stat = %d\n", stat);

    result = (char *)factory_dispatch(factory, type2, (void *)data2, strlen(data2));
    printf("result from worker2 = \"%s\"\n", result);

    result = (char *)factory_dispatch(factory, type1, (void *)data1, strlen(data1));
    printf("result from worker1 = \"%s\"\n", result);

    stat = factory_remove(factory, worker1);
    printf("removed worker1, stat = %d\n", stat);

    result = (char *)factory_dispatch(factory, type1, (void *)data1, strlen(data1));
    printf("result from worker1 = \"%s\"\n", result);

    worker1 = worker1_create(type1);
    stat = factory_add(factory, worker1);
    printf("added worker1, stat = %d\n", stat);

    result = (char *)factory_dispatch(factory, type1, (void *)data1, strlen(data1));
    printf("result from worker1 = \"%s\"\n", result);

    result = (char *)factory_dispatch(factory, type2, (void *)data2, strlen(data2));
    printf("result from worker2 = \"%s\"\n", result);

    stat = factory_destroy(factory);
    printf("factory destroyed, stat = %d\n", stat);

    return 0;

}

