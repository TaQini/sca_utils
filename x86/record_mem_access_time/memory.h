#ifndef x86_MEMORY_H
#define x86_MEMORY_H

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

#endif /*x86_MEMORY_H*/