/****************************************************************************
 * libs/libc/stdio/lib_vsscanf.c
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

#include <nuttx/streams.h>

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: vsscanf
 ****************************************************************************/

int vsscanf(FAR const char *buf, FAR const IPTR char *fmt, va_list ap)
{
  struct lib_meminstream_s meminstream;
  int n;

  /* Initialize a memory stream to freadm from the buffer */

  lib_meminstream(&meminstream, buf, INT_MAX);

  /* Then let lib_vscanf do the real work */

  n = lib_vscanf(&meminstream.common, NULL, fmt, ap);
  return n;
}
