/****************************************************************************
 * sched/group/group_setuptaskfiles.c
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

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>

#include <sched.h>
#include <assert.h>

#include <nuttx/fs/fs.h>
#include <nuttx/trace.h>

#include "sched/sched.h"
#include "group/group.h"

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: group_setuptaskfiles
 *
 * Description:
 *   Configure a newly allocated TCB so that it will inherit
 *   file descriptors and streams from the parent task.
 *
 * Input Parameters:
 *   tcb     - tcb of the new task.
 *   actions - The spawn file actions
 *   cloexec - Perform O_CLOEXEC on setup task files
 *
 * Returned Value:
 *   Zero (OK) is returned on success; A negated errno value is returned on
 *   failure.
 *
 * Assumptions:
 *
 ****************************************************************************/

int group_setuptaskfiles(FAR struct task_tcb_s *tcb,
                         FAR const posix_spawn_file_actions_t *actions,
                         bool cloexec)
{
  FAR struct task_group_s *group = tcb->cmn.group;
  int ret = OK;
#ifndef CONFIG_FDCLONE_DISABLE
  FAR struct tcb_s *rtcb;
#endif

  sched_trace_begin();
  DEBUGASSERT(group);
#ifndef CONFIG_DISABLE_PTHREAD
  DEBUGASSERT((tcb->cmn.flags & TCB_FLAG_TTYPE_MASK) !=
              TCB_FLAG_TTYPE_PTHREAD);
#endif

#ifndef CONFIG_FDCLONE_DISABLE

  /* the The file descriptors of kernel threads should be clean and
   * should not be generated based on user threads. Instead, an idle
   * thread should be chosen.
   */

  if ((tcb->cmn.flags & TCB_FLAG_TTYPE_MASK) == TCB_FLAG_TTYPE_KERNEL)
    {
      rtcb = &g_idletcb[this_cpu()].cmn;
    }
  else
    {
      rtcb = this_task();
    }

  /* Duplicate the parent task's file descriptors */

  DEBUGASSERT(rtcb->group);
  ret = files_duplist(&rtcb->group->tg_filelist,
                      &group->tg_filelist, actions, cloexec);
  if (ret >= 0 && actions != NULL)
    {
      ret = spawn_file_actions(&tcb->cmn, actions);
    }
#endif

  sched_trace_end();
  return ret;
}
