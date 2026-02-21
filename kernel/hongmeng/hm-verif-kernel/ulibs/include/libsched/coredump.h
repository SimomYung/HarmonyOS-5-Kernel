/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: coredump header file for user space
 * Author: Huawei OS Kernel Lab
 * Create: Fri May 27 20:00:00 2022
 */

#ifndef _LIBSCHED_COREDUMP_H
#define _LIBSCHED_COREDUMP_H

/*
 * DUMP_DISABLE: No dumping
 * DUMP_AS_USER: Dump as the user of process
 * DUMP_AS_ROOT: Dump as the root
 */
#define DUMP_DISABLE __U(0)
#define DUMP_AS_USER __U(1)
#define DUMP_AS_ROOT __U(2)

#endif
