#ifndef PROCESS_CALLS
#define PROCESS_CALLS

#include "./types.h"

#ifdef __cplusplus
extern "C" {
#endif

void process_calls(draw_call *calls, size_t n_calls);

#ifdef __cplusplus
}
#endif

#endif