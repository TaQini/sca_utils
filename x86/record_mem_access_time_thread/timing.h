#ifndef x64_TIMING_H
#define x64_TIMING_H

#include "memory.h"

uint64_t rdtsc(void);

// rdtsc
inline uint64_t rdtsc() {
    uint64_t a, d;
    x64_memory_barrier();
    asm volatile ("rdtsc" : "=a" (a), "=d" (d));
    x64_memory_barrier();
    return (d << 32) | a;
}

#endif /*x64_TIMING_H*/

