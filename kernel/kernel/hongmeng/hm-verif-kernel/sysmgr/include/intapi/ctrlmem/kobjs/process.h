/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: kobj process
 * Author: Huawei OS Kernel Lab
 * Create: Fri Aug 18 11:00:00 2023
 */

#ifndef INTAPI_CTRLMEM_KOBJS_PROCESS_H
#define INTAPI_CTRLMEM_KOBJS_PROCESS_H

#include <intapi/process/pid.h>
#include <intapi/ctrlmem/kobjctnr.h>
#include <hmkernel/ctrlmem/kobjs/process.h>

extern struct __ctrlmem_kobj_process_s *ctrlmem_kobj_process_spawn(void);

extern int ctrlmem_kobj_process_destroy(struct __ctrlmem_kobj_process_s *kobj);

extern void ctrlmem_kobj_process_set_pid(struct __ctrlmem_kobj_process_s *kobj, struct pid_s *pid);

extern void ctrlmem_kobj_process_set_init_load_pct(struct __ctrlmem_kobj_process_s *kobj, __u32 val);

extern void ctrlmem_kobj_process_set_load(struct __ctrlmem_kobj_process_s *kobj, __u64 val);

extern void ctrlmem_kobj_process_set_prev_running_time_ms(struct __ctrlmem_kobj_process_s *kobj, __u64 val);

extern void ctrlmem_kobj_process_set_prev_load(struct __ctrlmem_kobj_process_s *kobj, __u64 val);

extern void ctrlmem_kobj_process_set_boost(struct __ctrlmem_kobj_process_s *kobj, __u32 val);

#endif
