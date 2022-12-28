#include <stdio.h>
#include "timing.h"
#include "memory.h"
#include <pthread.h>

static int dummy_mem[4096];

// mem access and get time record
#define REPEAT 1000000
#define MAX_CYCLE 10000
uint64_t rpt[REPEAT];

volatile uint64_t counter = 0;

static void *thread_counter_func() {
    while (1) {
        asm volatile("1: add %0, %0, 1;"
                 "str %0, [%1];"
                 "bl 1b"::"r"(0ULL),
                          "r"(&counter));
    }
    pthread_exit(NULL);
}

int main(){
    uint64_t repeat = REPEAT;
    memset(rpt, 0, sizeof(rpt));
    volatile int *dummy = &(dummy_mem[2048]);
    pthread_t thread;
    pthread_create(&thread, NULL, thread_counter_func, NULL);

    for(int i = 0; i < repeat; i++){
        arm_v8_flush(dummy);
        arm_v8_memory_barrier();
        uint64_t start = counter;
        arm_v8_memory_barrier();

        arm_v8_access_memory(dummy);

        arm_v8_memory_barrier();
        uint64_t end = counter;
        arm_v8_memory_barrier();
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

