#include <stdio.h>
#include "timing.h"
#include "memory.h"

// timing.h
void arm_v8_timing_init(void);
uint64_t arm_v8_get_timing(void);
void arm_v8_timing_terminate(void);
void arm_v8_reset_timing(void);

// memory.h
void arm_v8_access_memory(void* pointer);
void arm_v8_memory_barrier(void);
void arm_v8_prefetch(void* pointer);

static int dummy_mem[4096];

// mem access and get time record
#define REPEAT 100000
size_t rpt[REPEAT];

uint64_t rdtsc(){
    arm_v8_memory_barrier();
    uint64_t t = arm_v8_get_timing();
    arm_v8_memory_barrier();
    return t;
}

int main(){
    volatile int *dummy = &(dummy_mem[2048]);
    arm_v8_timing_init();
    size_t repeat = REPEAT;
    for(int i = 0; i < repeat; i++){
        size_t start = rdtsc();
        arm_v8_access_memory(dummy);
        size_t end = rdtsc();
        size_t diff = end - start;
        // printf("%lu\n",diff );
        rpt[i] = diff;
    }
    for (int i = 0; i < repeat; ++i) {
        printf("%lu\n", rpt[i]);
    }
}

