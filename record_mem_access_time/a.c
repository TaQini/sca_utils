#include <stdio.h>
#include <stdint.h>
#include "timing.h"
#include "memory.h"

static int dummy_mem[4096];
static int l1_thrash[256 * 1024 / sizeof(int)];

void maccess(volatile void *p);
uint64_t rdtsc();

// mem access and get time record
#define REPEAT 100000
size_t rpt[REPEAT];
int main(){
	size_t repeat = REPEAT;
	volatile int *dummy = &(dummy_mem[2048]);
	for(int i = 0; i < repeat; i++){
        size_t start = rdtsc();
        maccess(dummy);
        size_t end = rdtsc();
        size_t diff = end - start;
        // printf("%lu\n",diff );
        rpt[i] = diff;
	}
	for (int i = 0; i < repeat; ++i) {
		printf("%lu\n", rpt[i]);
	}
}
