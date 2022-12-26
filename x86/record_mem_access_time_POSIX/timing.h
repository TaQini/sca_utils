#ifndef x64_TIMING_H
#define x64_TIMING_H

#include <time.h>
#include "memory.h"

uint64_t get_monotonic_time(void);

inline uint64_t get_monotonic_time(void){
  struct timespec t1;
  x64_memory_barrier();
  clock_gettime(CLOCK_MONOTONIC, &t1);
  x64_memory_barrier();
  return t1.tv_sec * 1000*1000*1000ULL + t1.tv_nsec;
}

#endif /*x64_TIMING_H*/

