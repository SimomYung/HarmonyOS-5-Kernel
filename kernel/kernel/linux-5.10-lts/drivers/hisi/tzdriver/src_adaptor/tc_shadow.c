/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */
#include "tc_shadow.h"
#include <linux/workqueue.h>
#include <linux/kthread.h>
#include <securec.h>
#include "smc_smp.h"
#include "tc_ns_log.h"
#include "tc_current_info.h"

static void start_shadow_work_func(struct kthread_work *kwork)
{
	struct task_struct *thread = shadow_work_func(kwork);
	if (!thread)
		return;
	wake_up_process(thread);
}

int smc_queue_shadow_worker(uint64_t target, const char *name)
{
	if (name == NULL)
		return -1;

	struct kthread_worker *ipi_worker = get_ipi_helper_worker();
	struct shadow_work work = {
		KTHREAD_WORK_INIT(work.kthwork, start_shadow_work_func),
		.target = target,
	};
	(void)strncpy_s(work.name, SHADOW_NAME_LEN, name, strnlen(name, SHADOW_NAME_LEN - 1));

	if (!ipi_worker)
		return -1;

#if (KERNEL_VERSION(4, 9, 0) > LINUX_VERSION_CODE)
	if (!queue_kthread_work(ipi_worker, &work.kthwork)) {
#else
	if (!kthread_queue_work(ipi_worker, &work.kthwork)) {
#endif
		tloge("ipi helper work fail queue, was already pending\n");
		return -1;
	}

#if (KERNEL_VERSION(4, 9, 0) > LINUX_VERSION_CODE)
	flush_kthread_work(&work.kthwork);
#else
	kthread_flush_work(&work.kthwork);
#endif
	return 0;
}
