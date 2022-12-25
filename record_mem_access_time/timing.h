#ifndef x86_TIMING_H
#define x86_TIMING_H

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

#endif /*x86_TIMING_H*/
