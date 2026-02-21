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
#include "tc_freeze.h"
#include <linux/freezer.h>
#include "tc_current_info.h"
#include "tc_ns_log.h"

/* to avoid hungtask or suspend fail, lock across REE-TEE should be interruptible */
int lock_freezable(struct mutex *m)
{
	int ret;

	if (!m)
		return -1;

	while (true) {
		ret = mutex_lock_interruptible(m);
		/* signal */
		if (ret == -EINTR) {
			/* kill signal */
			if (current_sigkill_pending())
				return ret;

			/* other signal or freezer */
			clear_thread_flag(TIF_SIGPENDING);
			freezable_schedule();
			tlogd("receive other signal or freezer, try relock");
			continue;
		} else {
			return ret;
		}
	}

	return 0;
}