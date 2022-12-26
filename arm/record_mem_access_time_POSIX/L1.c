#include <stdio.h>
#include "timing.h"
#include "memory.h"

static int dummy_mem[4096];

// mem access and get time record
#define REPEAT 1000000
#define MAX_CYCLE 10000
uint64_t rpt[REPEAT];

int main(){
    uint64_t repeat = REPEAT;
    memset(rpt, 0, sizeof(rpt));
    volatile int *dummy = &(dummy_mem[2048]);
    arm_v8_access_memory(dummy);
    for(int i = 0; i < repeat; i++){
        uint64_t start = get_monotonic_time();
        arm_v8_access_memory(dummy);
        uint64_t end = get_monotonic_time();
        uint64_t diff = end - start;
        // printf("%lu\n",diff );
        if(diff < MAX_CYCLE){
            rpt[i] = diff;
        }
    }
    for (int i = 0; i < repeat; ++i) {
        printf("%lu\n", rpt[i]);
    }
}

