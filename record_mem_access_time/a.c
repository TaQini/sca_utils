#include <stdio.h>
#include <stdint.h>

static int dummy_mem[4096];
static int l1_thrash[256 * 1024 / sizeof(int)];

// ---------------------------------------------------------------------------
uint64_t rdtsc() {
    uint64_t a, d;
    asm volatile ("mfence");
    asm volatile ("rdtsc" : "=a" (a), "=d" (d));
    asm volatile ("mfence");
    return (d << 32) | a;
}

// ---------------------------------------------------------------------------
uint64_t rdtscl() {
    uint64_t a, d;
    asm volatile ("lfence");
    asm volatile ("rdtsc" : "=a" (a), "=d" (d));
    asm volatile ("lfence");
    return (d << 32) | a;
}

// ---------------------------------------------------------------------------
void maccess(volatile void *p) {
    asm volatile ("movq (%0), %%rax\n"
    :
    : "c" (p)
    : "rax");
}

// ---------------------------------------------------------------------------
void flush(volatile void *p) {
    asm volatile ("clflush 0(%0)\n"
    :
    : "c" (p)
    : "rax");
}

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
