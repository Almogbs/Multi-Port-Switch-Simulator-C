#include <stdio.h>
#include <stdlib.h>

#include "common.h"
#include "switch.h"
#include "queue.h"

enum switch_sim_args {
    SWITCH_SIM_ARGS_ARRIVAL_TIME_INTERVAL = 1,
    SWITCH_SIM_ARGS_IN_PORTS_NUM,
    SWITCH_SIM_ARGS_OUT_PORTS_NUM,
};

static int switch_parse(int argc, char *argv[], struct switch_t *sw)
{
    int arg_idx, ret = 1;

    if (argc < 1 + SWITCH_SIM_ARGS_OUT_PORTS_NUM + 1) {
        SWITCH_SIM_ERR("Usage: %s <arrival_time_interval> <in_ports_num> <out_ports_num> <prob_matrix> <arrival_lamdas> <queue_sizes> <transmission_rates>\n", argv[0]);
        return 1;
    }

    sw->arrival_time_interval = atoi(argv[SWITCH_SIM_ARGS_ARRIVAL_TIME_INTERVAL]);
    if (sw->arrival_time_interval <= 0) {
        SWITCH_SIM_ERR("Invalid arrival_time_interval: %d\n", sw->arrival_time_interval);
        return 1;
    }

    sw->in_ports_num = atoi(argv[SWITCH_SIM_ARGS_IN_PORTS_NUM]);
    if (sw->in_ports_num <= 0) {
        SWITCH_SIM_ERR("Invalid in_ports_num: %d\n", sw->in_ports_num);
        goto out_free;
    }

    sw->out_ports_num = atoi(argv[SWITCH_SIM_ARGS_OUT_PORTS_NUM]);
    if (sw->out_ports_num <= 0) {
        SWITCH_SIM_ERR("Invalid out_ports_num: %d\n", sw->out_ports_num);
        goto out_free;
    }

    sw->prob_matrix = (float **)malloc(sw->in_ports_num * sizeof(float *));
    if (!sw->prob_matrix) {
        SWITCH_SIM_ERR("Failed to allocate memory for prob_matrix\n");
        goto out_free;
    }

    for (int i = 0; i < sw->in_ports_num; i++) {
        sw->prob_matrix[i] = (float *)malloc(sw->out_ports_num * sizeof(float));
        if (!sw->prob_matrix[i]) {
            SWITCH_SIM_ERR("Failed to allocate memory for prob_matrix[%d]\n", i);

            for (int j = 0; j < i; j++) {
                free(sw->prob_matrix[j]);
            }

            goto out_free;
        }
    }

    sw->arrival_lambdas = (float *)malloc(sw->in_ports_num * sizeof(float));
    if (!sw->arrival_lambdas) {
        SWITCH_SIM_ERR("Failed to allocate memory for arrival_lambdas\n");
        goto out_free;
    }

    sw->queue_sizes = (int *)malloc(sw->out_ports_num * sizeof(int));
    if (!sw->queue_sizes) {
        SWITCH_SIM_ERR("Failed to allocate memory for queue_sizes\n");
        goto out_free;
    }

    sw->transmission_rates = (float *)malloc(sw->out_ports_num * sizeof(float));
    if (!sw->transmission_rates) {
        SWITCH_SIM_ERR("Failed to allocate memory for transmission_rates\n");
        goto out_free;
    }

    SWITCH_SIM_DBG("arrival_time_interval: %d\n", sw->arrival_time_interval);
    SWITCH_SIM_DBG("in_ports_num: %d\n", sw->in_ports_num);
    SWITCH_SIM_DBG("out_ports_num: %d\n", sw->out_ports_num);

    arg_idx = SWITCH_SIM_ARGS_OUT_PORTS_NUM + 1;
    for (int i = 0; i < sw->in_ports_num; i++) {
        for (int j = 0; j < sw->out_ports_num; j++) {
            sw->prob_matrix[i][j] = atof(argv[arg_idx++]);
            SWITCH_SIM_DBG("prob_matrix[%d][%d]: %f\n", i, j, sw->prob_matrix[i][j]);
        }
    }

    for (int i = 0; i < sw->in_ports_num; i++) {
        sw->arrival_lambdas[i] = atof(argv[arg_idx++]);
        SWITCH_SIM_DBG("arrival_lambdas[%d]: %f\n", i, sw->arrival_lambdas[i]);
    }

    for (int i = 0; i < sw->out_ports_num; i++) {
        sw->queue_sizes[i] = atoi(argv[arg_idx++]);
        SWITCH_SIM_DBG("queue_sizes[%d]: %d\n", i, sw->queue_sizes[i]);
    }

    for (int i = 0; i < sw->out_ports_num; i++) {
        sw->transmission_rates[i] = atof(argv[arg_idx++]);
        SWITCH_SIM_DBG("transmission_rates[%d]: %f\n", i, sw->transmission_rates[i]);
    }
    
    sw->queues = (struct queue_t **)malloc(sw->out_ports_num * sizeof(struct queue_t *));
    if (!sw->queues) {
        SWITCH_SIM_ERR("Failed to allocate memory for queues\n");
        goto out_free;
    }

    for (int i = 0; i < sw->out_ports_num; i++) {
        sw->queues[i] = queue_init(sw->queue_sizes[i]);
        if (!sw->queues[i]) {
            SWITCH_SIM_ERR("Failed to initialize queue[%d]\n", i);

            for (int j = 0; j < i; j++) {
                queue_free(sw->queues[j]);
            }

            goto out_free;
        }
    }

    ret = 0;

out_free:
    switch_free(sw);
    return ret;
}

struct switch_t *switch_init(int argc, char *argv[])
{
    struct switch_t *sw = (struct switch_t *)malloc(sizeof(struct switch_t));
    if (!sw) {
        SWITCH_SIM_ERR("Failed to allocate memory for switch_t\n");
        return NULL;
    }

    if (switch_parse(argc, argv, sw)) {
        /* Avoid free here as switch_free() is being called in switch_parse() failures */
        return NULL;
    }

    return sw;
}

void switch_free(struct switch_t *sw)
{
    if (sw) {
        SWITCH_SIM_FREE_IF_NOT_NULL(sw->arrival_lambdas);
        SWITCH_SIM_FREE_IF_NOT_NULL(sw->queue_sizes);
        SWITCH_SIM_FREE_IF_NOT_NULL(sw->transmission_rates);
        if (sw->prob_matrix) {
            for (int i = 0; i < sw->in_ports_num; i++) {
                SWITCH_SIM_FREE_IF_NOT_NULL(sw->prob_matrix[i]);
            }
            SWITCH_SIM_FREE_IF_NOT_NULL(sw->prob_matrix);
        }
        if (sw->queues) {
            for (int i = 0; i < sw->out_ports_num; i++) {
                queue_free(sw->queues[i]);
            }
            free(sw->queues);
        }
    
        free(sw);
    }
}

void switch_start(struct switch_t *sw)
{
    if (!sw) {
        SWITCH_SIM_ERR("Invalid switch_t\n");
        return;
    }


}


