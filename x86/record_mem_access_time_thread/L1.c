#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "timing.h"
#include "memory.h"
#include <pthread.h>

static int dummy_mem[4096];
static int l1_thrash[256 * 1024 / sizeof(int)];

// mem access and get time record
#define REPEAT 1000000
#define MAX_CYCLE 10000
uint64_t rpt[REPEAT];

volatile uint64_t counter = 0;

static void *thread_counter_func() {
    asm volatile("1: inc %%ecx;"
                 "mov %%ecx,(%%rax);"
                 "jmp 1b"::"a"(&counter),
                           "c"(0ULL));
    pthread_exit(NULL);
}

int main(){
    uint64_t repeat = REPEAT;
    memset(rpt, 0, sizeof(rpt));
    volatile int *dummy = &(dummy_mem[2048]);
    x64_access_memory(dummy);
    pthread_t thread;
    pthread_create(&thread, NULL, thread_counter_func, NULL);

    for(int i = 0; i < repeat; i++){
        x64_memory_barrier();
        uint64_t start = counter;
        x64_memory_barrier();
        
        x64_access_memory(dummy);

        x64_memory_barrier();
        uint64_t end = counter;
        x64_memory_barrier();
        
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
