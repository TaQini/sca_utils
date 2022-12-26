#ifndef x64_MEMORY_H
#define x64_MEMORY_H

void x64_access_memory(volatile void *p);
void x64_memory_barrier(void);
void x64_flush(volatile void *p);

inline void x64_access_memory(volatile void *p) {
    asm volatile ("movq (%0), %%rax\n"
    :
    : "c" (p)
    : "rax");
}

inline void x64_memory_barrier(void) {
    asm volatile ("mfence");
// asm volatile ("lfence");
}

inline void x64_flush(volatile void *p) {
    asm volatile ("clflush 0(%0)\n"
    :
    : "c" (p)
    : "rax");
}

#endif /*x64_MEMORY_H*/