/* See LICENSE file for license and copyright information */

#ifndef ARM_V8_TIMING_H
#define ARM_V8_TIMING_H

#include <stdbool.h>
#include <assert.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <linux/perf_event.h>

#include <stdint.h>
#include "memory.h"

#define ARMV8_PMCR_E            (1 << 0)  /* Enable all counters */
#define ARMV8_PMCR_P            (1 << 1)  /* Reset all counters */
#define ARMV8_PMCR_C            (1 << 2)  /* Cycle counter reset */

#define ARMV8_PMUSERENR_EN      (1 << 0)  /* EL0 access enable */
#define ARMV8_PMUSERENR_CR      (1 << 2)  /* Cycle counter read enable */
#define ARMV8_PMUSERENR_ER      (1 << 3)  /* Event counter read enable */

#define ARMV8_PMCNTENSET_EL0_EN (1 << 31) /* Performance Monitors Count Enable Set register */

void arm_v8_timing_init(void);
uint64_t arm_v8_get_timing(void);
void arm_v8_timing_terminate(void);
void arm_v8_reset_timing(void);

int perf_init();
bool perf_terminate(int fd);
uint64_t perf_get_timing(int fd);
void perf_reset_timing(int fd);

// PMU
inline uint64_t arm_v8_get_timing(void) {
    uint64_t result = 0;
    arm_v8_memory_barrier();
    asm volatile("MRS %0, PMCCNTR_EL0" : "=r" (result));
    arm_v8_memory_barrier();
    return result;
}

inline void arm_v8_timing_init(void) {
    uint32_t value = 0;
    
    /* Enable Performance Counter */
    asm volatile("MRS %0, PMCR_EL0" : "=r" (value));
    value |= ARMV8_PMCR_E; /* Enable */
    value |= ARMV8_PMCR_C; /* Cycle counter reset */
    value |= ARMV8_PMCR_P; /* Reset all counters */
    asm volatile("MSR PMCR_EL0, %0" : : "r" (value));

    /* Enable cycle counter register */
    asm volatile("MRS %0, PMCNTENSET_EL0" : "=r" (value));
    value |= ARMV8_PMCNTENSET_EL0_EN;
    asm volatile("MSR PMCNTENSET_EL0, %0" : : "r" (value));
}

inline void arm_v8_timing_terminate(void) {
    uint32_t value = 0;
    uint32_t mask = 0;

    /* Disable Performance Counter */
    asm volatile("MRS %0, PMCR_EL0" : "=r" (value));
    mask = 0;
    mask |= ARMV8_PMCR_E; /* Enable */
    mask |= ARMV8_PMCR_C; /* Cycle counter reset */
    mask |= ARMV8_PMCR_P; /* Reset all counters */
    asm volatile("MSR PMCR_EL0, %0" : : "r" (value & ~mask));

    /* Disable cycle counter register */
    asm volatile("MRS %0, PMCNTENSET_EL0" : "=r" (value));
    mask = 0;
    mask |= ARMV8_PMCNTENSET_EL0_EN;
    asm volatile("MSR PMCNTENSET_EL0, %0" : : "r" (value & ~mask));
}

inline void arm_v8_reset_timing(void) {
    uint32_t value = 0;
    asm volatile("MRS %0, PMCR_EL0" : "=r" (value));
    value |= ARMV8_PMCR_C; /* Cycle counter reset */
    asm volatile("MSR PMCR_EL0, %0" : : "r" (value));
}

// perf_enevt

inline int perf_init() {
    static struct perf_event_attr attr;
    attr.type = PERF_TYPE_HARDWARE;
    attr.config = PERF_COUNT_HW_CPU_CYCLES;
    attr.size = sizeof(attr);
    attr.exclude_kernel = 1;
    attr.exclude_hv = 1;
    attr.exclude_callchain_kernel = 1;

    int fd = syscall(__NR_perf_event_open, &attr, 0, -1, -1, 0);
    assert(fd >= 0 && "if this assertion fails you have no perf event interface available for the userspace. install a different kernel/rom."); // if this assertion fails you have no perf event interface available for the userspace. install a different kernel/rom.

    return fd;
}

inline bool perf_terminate(int fd) {
    close(fd);
    return true;
}

inline uint64_t perf_get_timing(int fd) {
    long long result = 0;
    arm_v8_memory_barrier();
    if (read(fd, &result, sizeof(result)) < (ssize_t) sizeof(result)) {
        return 0;
    }
    arm_v8_memory_barrier();
    return result;
}

inline void perf_reset_timing(int fd) {
    ioctl(fd, PERF_EVENT_IOC_RESET, 0);
}

#endif    /*ARM_V8_TIMING_H*/

