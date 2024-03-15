#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"
#include "switch.h"
#include "queue.h"


int main(int argc, char *argv[])
{
    struct switch_t *sw = switch_init(argc, argv);

    if (!sw) {
        SWITCH_SIM_ERR("Failed to initialize switch\n");
        return 1;
    }

    switch_start(sw);
    
    switch_free(sw);

    return 0;
}