/* See LICENSE file for license and copyright information */

#ifndef ARM_V8_TIMING_H
#define ARM_V8_TIMING_H

#include <stdint.h>
#include <time.h>
#include "memory.h"

uint64_t get_monotonic_time(void);

inline uint64_t get_monotonic_time(void){
  struct timespec t1;
  arm_v8_memory_barrier();
  clock_gettime(CLOCK_MONOTONIC, &t1);
  arm_v8_memory_barrier();
  return t1.tv_sec * 1000*1000*1000ULL + t1.tv_nsec;
}

#endif    /*ARM_V8_TIMING_H*/
