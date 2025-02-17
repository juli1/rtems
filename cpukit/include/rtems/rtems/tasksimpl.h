/**
 * @file
 *
 * @ingroup RTEMSImplClassicTask
 *
 * @brief This header file provides the implementation interfaces of
 *   the @ref RTEMSImplClassicTask.
 */

/*  COPYRIGHT (c) 1989-2014.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.org/license/LICENSE.
 */

#ifndef _RTEMS_RTEMS_TASKSIMPL_H
#define _RTEMS_RTEMS_TASKSIMPL_H

#include <rtems/rtems/tasksdata.h>
#include <rtems/score/objectimpl.h>
#include <rtems/score/schedulerimpl.h>
#include <rtems/score/threadimpl.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup RTEMSImplClassicTask Task Manager
 *
 * @ingroup RTEMSImplClassic
 *
 * @brief This group contains the Task Manager implementation.
 *
 * @{
 */

/**
 *  @brief RTEMS User Task Initialization
 *
 *  This routine creates and starts all configured user
 *  initialization threads.
 */
void _RTEMS_tasks_Initialize_user_tasks( void );

typedef rtems_status_code ( *RTEMS_tasks_Prepare_stack )(
  Thread_Configuration *,
  const rtems_task_config *
);

rtems_status_code _RTEMS_tasks_Create(
  const rtems_task_config   *config,
  rtems_id                  *id,
  RTEMS_tasks_Prepare_stack  prepare_stack
);

RTEMS_INLINE_ROUTINE Thread_Control *_RTEMS_tasks_Allocate(void)
{
  _Objects_Allocator_lock();

  _Thread_Kill_zombies();

  return (Thread_Control *)
    _Objects_Allocate_unprotected( &_RTEMS_tasks_Information.Objects );
}

/**
 * @brief Converts the RTEMS API priority to the corresponding SuperCore
 * priority and validates it.
 *
 * The RTEMS API system priority is accepted as valid.
 *
 * @param[in] scheduler The scheduler instance.
 * @param[in] priority The RTEMS API priority to convert and validate.
 * @param[out] valid Indicates if the RTEMS API priority is valid and a
 *   corresponding SuperCore priority in the specified scheduler instance
 *   exists.
 *
 * @return The corresponding SuperCore priority.
 */
RTEMS_INLINE_ROUTINE Priority_Control _RTEMS_Priority_To_core(
  const Scheduler_Control *scheduler,
  rtems_task_priority      priority,
  bool                    *valid
)
{
  *valid = ( priority <= scheduler->maximum_priority );

  return _Scheduler_Map_priority( scheduler, (Priority_Control) priority );
}

/**
 * @brief Converts the SuperCore priority to the corresponding RTEMS API
 * priority.
 *
 * @param[in] scheduler The scheduler instance.
 * @param[in] priority The SuperCore priority to convert.
 *
 * @return The corresponding RTEMS API priority.
 */
RTEMS_INLINE_ROUTINE rtems_task_priority _RTEMS_Priority_From_core(
  const Scheduler_Control *scheduler,
  Priority_Control         priority
)
{
  return (rtems_task_priority)
    _Scheduler_Unmap_priority( scheduler, priority );
}

/**@}*/

/**
 * @defgroup RTEMSImplClassicScheduler Scheduler Manager
 *
 * @ingroup RTEMSImplClassic
 *
 * @brief This group contains the Scheduler Manager implementation.
 */

#if defined(RTEMS_MULTIPROCESSING)
#include <rtems/rtems/taskmp.h>
#endif

#ifdef __cplusplus
}
#endif

#endif
/* end of include file */
