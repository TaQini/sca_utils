#ifndef x64_TIMING_H
#define x64_TIMING_H
#include <stdbool.h>
#include <assert.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <linux/perf_event.h>
#include "memory.h"

uint64_t rdtsc(void);
int perf_init();
bool perf_terminate(int fd);
uint64_t perf_get_timing(int fd);
void perf_reset_timing(int fd);

// rdtsc
inline uint64_t rdtsc() {
    uint64_t a, d;
    x64_memory_barrier();
    asm volatile ("rdtsc" : "=a" (a), "=d" (d));
    x64_memory_barrier();
    return (d << 32) | a;
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
    x64_memory_barrier();
    if (read(fd, &result, sizeof(result)) < (ssize_t) sizeof(result)) {
        return 0;
    }
    x64_memory_barrier();
    return result;
}

inline void perf_reset_timing(int fd) {
    ioctl(fd, PERF_EVENT_IOC_RESET, 0);
}

#endif /*x64_TIMING_H*/

