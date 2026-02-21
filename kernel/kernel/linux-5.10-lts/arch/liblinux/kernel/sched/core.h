/* SPDX-License-Identifier: GPL-2.0-only */
#ifndef __CORE_H__
#define __CORE_H__

/*
 * The enum variable below is used to define the kthread creation/dispatch time and hung detect state.
 * TASK_STATE_INITIAL: The kthread is created and not woken.
 * TASK_STATE_NO_HUNG_DETECT: The kthread will not be set with hung detect flag when it schedule with UNINTERRUPTIBLE.
 * TASK_STATE_WAKEUP: If the kthread is woken during IGNORE_STAGE of bootup when hung detect is not work,
 *                    its state is changed to TASK_STATE_NO_HUNG_DETECT when it schedule with UNINTERRUPTIBLE during IGNORE_STAGE.
 *                    If the kthread is woken after IGNORE_STAGE of bootup when hung detect start work,
 *                    it needs to be compared with whitelist of hung detect.
 * TASK_STATE_WHITELIST_CHECKED: The kthread is not in whitelist and will be set with hung detect flag.
 */
enum {
	TASK_STATE_INITIAL = 0,
	TASK_STATE_WAKEUP,
	TASK_STATE_WHITELIST_CHECKED,
	TASK_STATE_NO_HUNG_DETECT,
};

#endif /* __CORE_H__ */
