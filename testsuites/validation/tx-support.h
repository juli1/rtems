/* SPDX-License-Identifier: BSD-2-Clause */

/**
 * @file
 *
 * @ingroup RTEMSTestSuites
 *
 * @brief This header file provides the support functions for the validation
 *   test cases.
 */

/*
 * Copyright (C) 2021 embedded brains GmbH (http://www.embedded-brains.de)
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _TX_SUPPORT_H
#define _TX_SUPPORT_H

#include <rtems.h>
#include <rtems/irq-extension.h>
#include <rtems/score/atomic.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup RTEMSTestSuites
 *
 * @{
 */

typedef enum {
  PRIO_PSEUDO_ISR,
  PRIO_ULTRA_HIGH,
  PRIO_VERY_HIGH,
  PRIO_HIGH,
  PRIO_NORMAL,
  PRIO_LOW,
  PRIO_VERY_LOW,
  PRIO_ULTRA_LOW
} Priority;

/**
 * @brief This constants represents an invalid RTEMS object identifier.
 */
#define INVALID_ID 0xfffffffd

#define CreateTask( name, priority ) \
  DoCreateTask( \
    rtems_build_name( name[ 0 ], name[ 1 ], name[ 2 ], name[ 3 ] ), \
    priority \
  )

rtems_id DoCreateTask( rtems_name name, rtems_task_priority priority );

void StartTask( rtems_id id, rtems_task_entry entry, void *arg );

void DeleteTask( rtems_id id );

rtems_event_set ReceiveAnyEvents( void );

void SendEvents( rtems_id id, rtems_event_set events );

rtems_mode GetMode( void );

rtems_mode SetMode( rtems_mode set, rtems_mode mask );

rtems_task_priority GetPriority( rtems_id id );

rtems_task_priority SetPriority( rtems_id id, rtems_task_priority priority );

rtems_task_priority GetSelfPriority( void );

rtems_task_priority SetSelfPriority( rtems_task_priority priority );

void RestoreRunnerASR( void );

void RestoreRunnerMode( void );

void RestoreRunnerPriority( void );

typedef struct {
  Chain_Node node;
  void ( *handler )( void * );
  void *arg;
  Atomic_Uint done;
} CallWithinISRRequest;

void CallWithinISR( void ( *handler )( void * ), void *arg );

void CallWithinISRSubmit( CallWithinISRRequest *request );

void CallWithinISRWait( const CallWithinISRRequest *request );

rtems_vector_number GetValidInterruptVectorNumber(
  const rtems_interrupt_attributes *required
);

rtems_vector_number GetTestableInterruptVector( void );

bool HasInterruptVectorEntriesInstalled( rtems_vector_number vector );

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* _TX_SUPPORT_H */
