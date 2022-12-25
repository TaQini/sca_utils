#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/syscall.h>
#include <sched.h>
#include <pthread.h>
#include <sys/mman.h>

#include <math.h>
#include <pthread.h>
#include <sched.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>

// #define ADDR_PTR uint64_t
// #define CYCLES uint32_t

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

bool bind_to_cpu(size_t cpu){
  cpu_set_t mask;
  CPU_ZERO(&mask);
  CPU_SET(cpu, &mask);
  
  if (sched_setaffinity(0, sizeof(mask), &mask) == -1) {
    return false;
  } else {
    return true;
  }
}

#define REPEAT 1000000
size_t rpt[REPEAT];
int main(){
    bind_to_cpu(23);
	/*size_t repeat = REPEAT;
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
	}*/
    size_t number_of_cpus = sysconf(_SC_NPROCESSORS_ONLN);
    printf("%lu\n",number_of_cpus);
}
