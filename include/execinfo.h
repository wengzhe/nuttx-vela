/****************************************************************************
 * include/execinfo.h
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

#ifndef __INCLUDE_EXECINFO_H
#define __INCLUDE_EXECINFO_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/sched.h>

#include <sys/types.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* 3: ' 0x' prefix */

#define BACKTRACE_PTR_FMT_WIDTH ((int)sizeof(uintptr_t) * 2 + 3)

#define backtrace(buffer, size) sched_backtrace(_SCHED_GETTID(),      \
                                                buffer, size, 0)
#define dump_stack()            sched_dumpstack(_SCHED_GETTID())

/* Format a backtrace into a buffer for dumping. */

#define backtrace_format(buf, buflen, backtrace, depth)               \
do                                                                    \
  {                                                                   \
    FAR const char *format = " %0*p";                                 \
    int i;                                                            \
    for (i = 0; i < depth && backtrace[i]; i++)                       \
      {                                                               \
        snprintf(buf + i * BACKTRACE_PTR_FMT_WIDTH,                   \
                 buflen - i * BACKTRACE_PTR_FMT_WIDTH,                \
                 format, BACKTRACE_PTR_FMT_WIDTH - 1, backtrace[i]);  \
      }                                                               \
  }                                                                   \
while(0)

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

#undef EXTERN
#if defined(__cplusplus)
#define EXTERN extern "C"
extern "C"
{
#else
#define EXTERN extern
#endif

FAR char **backtrace_symbols(FAR void *const *buffer, int size);
void backtrace_symbols_fd(FAR void *const *buffer, int size, int fd);

#undef EXTERN
#if defined(__cplusplus)
}
#endif

#endif /* __INCLUDE_EXECINFO_H */
