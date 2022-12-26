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
    volatile int *dummy = &(dummy_mem[2048]);
    int fd = perf_init();
    x64_access_memory(dummy);
    for(int i = 0; i < repeat; i++){
        x64_flush(dummy);
        asm volatile("xor %%rax, %%rax\ncpuid":: : "rax", "rbx", "rcx", "rdx");
        uint64_t start = rdtsc();
        x64_access_memory(dummy);
        uint64_t end = rdtsc();
        asm volatile("xor %%rax, %%rax\ncpuid":: : "rax", "rbx", "rcx", "rdx");
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
