#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "timing.h"
#include "memory.h"

static int dummy_mem[4096];
static int l1_thrash[256 * 1024 / sizeof(int)];

// mem access and get time record
#define REPEAT 1000000
#define MAX_CYCLE 10000
uint64_t rpt[REPEAT];

int main(){
    uint64_t repeat = REPEAT;
    memset(rpt, 0, sizeof(rpt));
    int fd = perf_init();
    volatile int *dummy = &(dummy_mem[2048]);
    for(int i = 0; i < repeat; i++){
        // access dummy -> L1 (hit or not)
        x64_access_memory(dummy);
        // flush L1: padding fill -> L1
        asm volatile("xor %%rax, %%rax\ncpuid":: : "rax", "rbx", "rcx", "rdx");
        memset(l1_thrash, i & 0xff, sizeof(l1_thrash));
        asm volatile("xor %%rax, %%rax\ncpuid":: : "rax", "rbx", "rcx", "rdx");
        // access dummy again -> L1 (miss)
        uint64_t start = perf_get_timing(fd);
        x64_access_memory(dummy);
        uint64_t end = perf_get_timing(fd);
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
