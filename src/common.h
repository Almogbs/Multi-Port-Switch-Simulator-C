#ifndef __COMMON_H__
#define __COMMON_H__

#define SWITCH_SIM_DEBUG 1
#define SWITCH_SIM_FMT "switch-sim: "
#define SWITCH_SIM_ERR(fmt, ...) fprintf(stderr, SWITCH_SIM_FMT fmt, ##__VA_ARGS__)
#define SWITCH_SIM_DBG(fmt, ...) if (SWITCH_SIM_DEBUG) { fprintf(stdout, SWITCH_SIM_FMT fmt, ##__VA_ARGS__); }

#define SWITCH_SIM_FREE_IF_NOT_NULL(ptr) if (ptr) { free(ptr); ptr = NULL; }

#endif /* __COMMON_H__ */