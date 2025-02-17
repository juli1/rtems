/**
 * @file
 *
 * @ingroup RTEMSScoreProfiling
 *
 * @brief This header file provides the interfaces of the
 *   @ref RTEMSScoreProfiling.
 */

/*
 * Copyright (c) 2014 embedded brains GmbH.  All rights reserved.
 *
 *  embedded brains GmbH
 *  Dornierstr. 4
 *  82178 Puchheim
 *  Germany
 *  <rtems@embedded-brains.de>
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rtems.org/license/LICENSE.
 */

#ifndef _RTEMS_SCORE_PROFILING
#define _RTEMS_SCORE_PROFILING

#include <rtems/score/percpu.h>
#include <rtems/score/isrlock.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * @defgroup RTEMSScoreProfiling Profiling Support
 * 
 * @ingroup RTEMSScore
 *
 * @brief This group contains the implementation to support profiling.
 *
 * @{
 */

/**
 * @brief Disables the thread dispatch if the previous thread dispatch
 *      disable level is zero.
 *
 * @param[out] cpu The cpu control.
 * @param previous_thread_dispatch_disable_level The dispatch disable
 *      level of the previous thread.
 */
static inline void _Profiling_Thread_dispatch_disable(
  Per_CPU_Control *cpu,
  uint32_t previous_thread_dispatch_disable_level
)
{
#if defined( RTEMS_PROFILING )
  if ( previous_thread_dispatch_disable_level == 0 ) {
    Per_CPU_Stats *stats = &cpu->Stats;

    stats->thread_dispatch_disabled_instant = _CPU_Counter_read();
    ++stats->thread_dispatch_disabled_count;
  }
#else
  (void) cpu;
  (void) previous_thread_dispatch_disable_level;
#endif
}

/**
 * @brief Disables the thread dispatch.
 *
 * Only if the previous thread dispatch disable level is zero.  This
 * method also takes into account the lock_context.
 *
 * @param[out] cpu The cpu control.
 * @param previous_thread_dispatch_disable_level The dispatch disable
 *      level of the previous thread.
 * @param lock_context The lock context.
 */
static inline void _Profiling_Thread_dispatch_disable_critical(
  Per_CPU_Control        *cpu,
  uint32_t                previous_thread_dispatch_disable_level,
  const ISR_lock_Context *lock_context
)
{
#if defined( RTEMS_PROFILING )
  if ( previous_thread_dispatch_disable_level == 0 ) {
    Per_CPU_Stats *stats = &cpu->Stats;

    stats->thread_dispatch_disabled_instant = lock_context->ISR_disable_instant;
    ++stats->thread_dispatch_disabled_count;
  }
#else
  (void) cpu;
  (void) previous_thread_dispatch_disable_level;
  (void) lock_context;
#endif
}

/**
 * @brief Enables the thread dispatch.
 *
 * Only if the @a new_thread_dispatch_disable_level is 0.
 *
 * @param[out] cpu The cpu control.
 * @param new_thread_dispatch_disable_level The dispatch disable level
 *      of the new thread.
 */
static inline void _Profiling_Thread_dispatch_enable(
  Per_CPU_Control *cpu,
  uint32_t new_thread_dispatch_disable_level
)
{
#if defined( RTEMS_PROFILING )
  if ( new_thread_dispatch_disable_level == 0 ) {
    Per_CPU_Stats *stats = &cpu->Stats;
    CPU_Counter_ticks now = _CPU_Counter_read();
    CPU_Counter_ticks delta = _CPU_Counter_difference(
      now,
      stats->thread_dispatch_disabled_instant
    );

    stats->total_thread_dispatch_disabled_time += delta;

    if ( stats->max_thread_dispatch_disabled_time < delta ) {
      stats->max_thread_dispatch_disabled_time = delta;
    }
  }
#else
  (void) cpu;
  (void) new_thread_dispatch_disable_level;
#endif
}

/**
 * @brief Updates the maximum interrupt delay.
 *
 * @param[out] cpu The cpu control.
 * @param interrupt_delay The new interrupt delay.
 */
static inline void _Profiling_Update_max_interrupt_delay(
  Per_CPU_Control *cpu,
  CPU_Counter_ticks interrupt_delay
)
{
#if defined( RTEMS_PROFILING )
  Per_CPU_Stats *stats = &cpu->Stats;

  if ( stats->max_interrupt_delay < interrupt_delay ) {
    stats->max_interrupt_delay = interrupt_delay;
  }
#else
  (void) cpu;
  (void) interrupt_delay;
#endif
}

/**
 * @brief Updates the interrupt profiling statistics.
 *
 * Must be called with the interrupt stack and before the thread dispatch
 * disable level is decremented.
 *
 * @param cpu The cpu control.
 * @param interrupt_entry_instant The instant that the interrupt occurred.
 * @param interrupt_exit_instant The instant in which the interrupt was exited.
 */
void _Profiling_Outer_most_interrupt_entry_and_exit(
  Per_CPU_Control *cpu,
  CPU_Counter_ticks interrupt_entry_instant,
  CPU_Counter_ticks interrupt_exit_instant
);

/** @} */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _RTEMS_SCORE_PROFILING */
