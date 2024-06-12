/****************************************************************************
 * sched/pthread/pthread.h
 *
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.  The
 * ASF licenses this file to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance with the
 * License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  See the
 * License for the specific language governing permissions and limitations
 * under the License.
 *
 ****************************************************************************/

#ifndef __SCHED_PTHREAD_PTHREAD_H
#define __SCHED_PTHREAD_PTHREAD_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>

#include <sys/types.h>
#include <stdint.h>
#include <stdbool.h>
#include <pthread.h>

#include <nuttx/compiler.h>
#include <nuttx/semaphore.h>
#include <nuttx/sched.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#ifdef CONFIG_PTHREAD_MUTEX_TYPES
#  define mutex_init(m)          nxrmutex_init(m)
#  define mutex_destroy(m)       nxrmutex_destroy(m)
#  define mutex_is_hold(m)       nxrmutex_is_hold(m)
#  define mutex_is_locked(m)     nxrmutex_is_locked(m)
#  define mutex_is_recursive(m)  nxrmutex_is_recursive(m)
#  define mutex_get_holder(m)    nxrmutex_get_holder(m)
#  define mutex_reset(m)         nxrmutex_reset(m)
#  define mutex_unlock(m)        nxrmutex_unlock(m)
#  define mutex_lock(m)          nxrmutex_lock(m)
#  define mutex_trylock(m)       nxrmutex_trylock(m)
#  define mutex_breaklock(m,v)   nxrmutex_breaklock(m,v)
#  define mutex_restorelock(m,v) nxrmutex_restorelock(m,v)
#  define mutex_clocklock(m,t)   nxrmutex_clocklock(m,CLOCK_REALTIME,t)
#else
#  define mutex_init(m)          nxmutex_init(m)
#  define mutex_destroy(m)       nxmutex_destroy(m)
#  define mutex_is_hold(m)       nxmutex_is_hold(m)
#  define mutex_is_recursive(m)  (false)
#  define mutex_is_locked(m)     nxmutex_is_locked(m)
#  define mutex_get_holder(m)    nxmutex_get_holder(m)
#  define mutex_reset(m)         nxmutex_reset(m)
#  define mutex_unlock(m)        nxmutex_unlock(m)
#  define mutex_lock(m)          nxmutex_lock(m)
#  define mutex_trylock(m)       nxmutex_trylock(m)
#  define mutex_breaklock(m,v)   nxmutex_breaklock(m, v)
#  define mutex_restorelock(m,v) nxmutex_restorelock(m, v)
#  define mutex_clocklock(m,t)   nxmutex_clocklock(m,CLOCK_REALTIME,t)
#endif

/****************************************************************************
 * Public Type Declarations
 ****************************************************************************/

/* The following defines an entry in the pthread logic's local data set.
 * Note that this structure is used to implemented a singly linked list.
 * This structure is used (instead of, say, a binary search tree) because
 * the data set will be searched using the pid as a key -- a process IDs will
 * always be created in a montonically increasing fashion.
 */

struct join_s
{
  FAR struct join_s *next;       /* Implements link list */
  uint8_t            crefs;      /* Reference count */
  bool               detached;   /* true: pthread_detached'ed */
  bool               terminated; /* true: detach'ed+exit'ed */
  pthread_t          thread;     /* Includes pid */
  sem_t              exit_sem;   /* Implements join */
  sem_t              data_sem;   /* Implements join */
  pthread_addr_t     exit_value; /* Returned data */
};

/****************************************************************************
 * Public Data
 ****************************************************************************/

#ifdef __cplusplus
#define EXTERN extern "C"
extern "C"
{
#else
#define EXTERN extern
#endif

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

struct pthread_tcb_s; /* Forward reference */
struct task_group_s;  /* Forward reference */

int pthread_setup_scheduler(FAR struct pthread_tcb_s *tcb, int priority,
                            start_t start, pthread_startroutine_t entry,
                            FAR struct tcb_s *rtcb);

int pthread_completejoin(pid_t pid, FAR void *exit_value);
void pthread_destroyjoin(FAR struct task_group_s *group,
                         FAR struct join_s *pjoin);
int pthread_findjoininfo(FAR struct task_group_s *group,
                         pid_t pid, FAR struct join_s **join);
void pthread_release(FAR struct task_group_s *group);

#ifndef CONFIG_PTHREAD_MUTEX_UNSAFE
int pthread_mutex_take(FAR struct pthread_mutex_s *mutex,
                       FAR const struct timespec *abs_timeout);
int pthread_mutex_trytake(FAR struct pthread_mutex_s *mutex);
int pthread_mutex_give(FAR struct pthread_mutex_s *mutex);
int pthread_mutex_breaklock(FAR struct pthread_mutex_s *mutex,
                            FAR unsigned int *breakval);
int pthread_mutex_restorelock(FAR struct pthread_mutex_s *mutex,
                              unsigned int breakval);
void pthread_mutex_inconsistent(FAR struct tcb_s *tcb);
#else
#  define pthread_mutex_take(m,abs_timeout) -mutex_clocklock(&(m)->mutex, \
                                                             abs_timeout)
#  define pthread_mutex_trytake(m)          -mutex_trylock(&(m)->mutex)
#  define pthread_mutex_give(m)             -mutex_unlock(&(m)->mutex)
#  define pthread_mutex_breaklock(m,v)      -mutex_breaklock(&(m)->mutex,v)
#  define pthread_mutex_restorelock(m,v)    -mutex_restorelock(&(m)->mutex,v)
#endif

#ifdef CONFIG_PTHREAD_MUTEX_TYPES
int pthread_mutexattr_verifytype(int type);
#endif

#undef EXTERN
#ifdef __cplusplus
}
#endif

#endif /* __SCHED_PTHREAD_PTHREAD_H */
