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
void arm_v8_flush(void* address);

uint64_t rdtsc(){
    arm_v8_memory_barrier();
    uint64_t t = arm_v8_get_timing();
    arm_v8_memory_barrier();
    return t;
}

static int dummy_mem[4096];

// mem access and get time record
#define REPEAT 1000000
#define MAX_CYCLE 10000
size_t rpt[REPEAT];

int main(){
    size_t repeat = REPEAT;
    memset(rpt, 0, sizeof(rpt));
    volatile int *dummy = &(dummy_mem[2048]);
    arm_v8_timing_init();
    arm_v8_access_memory(dummy);
    for(int i = 0; i < repeat; i++){
        size_t start = rdtsc();
        arm_v8_access_memory(dummy);
        size_t end = rdtsc();
        size_t diff = end - start;
        // printf("%lu\n",diff );
        if(diff < MAX_CYCLE){
            rpt[i] = diff;
        }
    }
    for (int i = 0; i < repeat; ++i) {
        printf("%lu\n", rpt[i]);
    }
}

