/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2024. All rights reserved.
 * Description: dubai cpu stats
 */

#ifndef DUBAI_CPU_STATS_H
#define DUBAI_CPU_STATS_H

#include <linux/sched.h>

struct cpu_total_inst_data {
    uint64_t cpu_total_inst;
    uint32_t cpu_num;
} __packed;

long dubai_ioctl_cpu(unsigned int cmd, void __user *argp);
void dubai_proc_cputime_init(void);
void dubai_proc_cputime_exit(void);
#ifdef CONFIG_LIBLINUX
int liblinux_pal_cpu_inst_init(void);
void liblinux_pal_cpu_inst_exit(void);
#endif

#ifndef CONFIG_LIBLINUX
int dubai_cpu_inst_init(void);
void dubai_cpu_inst_exit(void);
#endif // CONFIG_LIBLINUX

#ifdef CONFIG_HUAWEI_DUBAI
void dubai_add_cpu_inst(int cpu, u64 inst);
#else
void dubai_add_cpu_inst(int cpu, u64 inst) {}
#endif

#endif // DUBAI_CPU_STATS_H
