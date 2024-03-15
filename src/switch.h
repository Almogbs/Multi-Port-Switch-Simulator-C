#ifndef __SWITCH_H__
#define __SWITCH_H__

struct switch_t {
    int arrival_time_interval;
    int in_ports_num;
    int out_ports_num;
    float **prob_matrix;
    float *arrival_lambdas;
    int *queue_sizes;
    float *transmission_rates;
    struct queue_t **queues;
    int *total_packets_per_queue;
};

struct switch_t *switch_init(int argc, char *argv[]);

void switch_free(struct switch_t *sw);

void switch_start(struct switch_t *sw);


#endif /* __SWITCH_H__ */