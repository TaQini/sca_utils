/* See LICENSE file for license and copyright information */

#ifndef ARM_v8_MEMORY_H
#define ARM_v8_MEMORY_H

void arm_v8_access_memory(void* pointer);
void arm_v8_memory_barrier(void);
void arm_v8_prefetch(void* pointer);
void arm_v8_flush(void* address);

inline void arm_v8_access_memory(void* pointer) {
    volatile uint32_t value;
    asm volatile ("LDR %0, [%1]\n\t"
            : "=r" (value)
            : "r" (pointer)
            );
}

inline void arm_v8_memory_barrier(void) {
    asm volatile ("DSB SY");
    asm volatile ("ISB");
}

inline void arm_v8_prefetch(void* pointer) {
    asm volatile ("PRFM PLDL3KEEP, [%x0]" :: "p" (pointer));
    asm volatile ("PRFM PLDL2KEEP, [%x0]" :: "p" (pointer));
    asm volatile ("PRFM PLDL1KEEP, [%x0]" :: "p" (pointer));
}

inline void arm_v8_flush(void* address) {
    asm volatile ("DC CIVAC, %0" :: "r"(address));
    asm volatile ("DSB ISH");
    asm volatile ("ISB");
}

#endif    /*ARM_v8_MEMORY_H*/

