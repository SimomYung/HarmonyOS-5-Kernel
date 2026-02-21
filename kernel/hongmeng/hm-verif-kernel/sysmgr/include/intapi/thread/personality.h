/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 * Description: Definition of personality (intapi)
 * Author: Huawei OS Kernel Lab
 * Create: Tue May 07 15:51:14 2024
 */

#ifndef SYSMGR_INTAPI_THREAD_PERSONALITY_H
#define SYSMGR_INTAPI_THREAD_PERSONALITY_H

#define ADDR_NO_RANDOMIZE 0x0040000  /* disable randomization of VA space */
void set_exec_current(struct thread_s *thread);
void clear_exec_current(void);
#endif
