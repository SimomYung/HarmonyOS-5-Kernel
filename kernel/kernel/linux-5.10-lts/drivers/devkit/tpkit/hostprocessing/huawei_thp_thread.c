/*
 * Huawei Touchscreen Driver
 *
 * Copyright (c) 2024-2024 Huawei Technologies Co., Ltd.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 */

#include "huawei_thp.h"

static int thp_cmd_sync_init(struct thp_cmd_node *cmd, int timeout)
{
	struct thp_cmd_sync *sync = NULL;

	if (timeout == 0) {
		cmd->sync = NULL;
		return 0;
	}
	sync = kzalloc(sizeof(*sync), GFP_KERNEL);
	if (sync == NULL) {
		thp_log_err("failed to kzalloc completion\n");
		return -ENOMEM;
	}
	init_completion(&sync->done);
	atomic_set(&sync->timeout_flag, TS_NOT_TIMEOUT);
	cmd->sync = sync;
	return 0;
}

static int put_one_cmd_check(struct thp_cmd_node *cmd)
{
    struct thp_core_data *cd = NULL;

    if (cmd == NULL) {
		thp_log_err("%s:null pointer\n", __func__);
		return -EINVAL;
	}
	cd = thp_get_core_data(cmd->cmd_param.thread_id);
	if ((!atomic_read(&cd->register_flag)) &&
		(cmd->command != TS_CHIP_DETECT)) {
		thp_log_err("%s: not initialize\n", __func__);
		return -EINVAL;
	}
    return 0;
}

int thp_put_one_cmd(struct thp_cmd_node *cmd, int timeout)
{
	int error = -EIO;
	unsigned long flags;
	struct thp_cmd_queue *q = NULL;
	struct thp_core_data *cd = NULL;

	if (put_one_cmd_check(cmd)) {
		thp_log_err("%s:input invalid\n", __func__);
		goto out;
	}
	cd = thp_get_core_data(cmd->cmd_param.thread_id);
	if (thp_cmd_sync_init(cmd, timeout)) {
		thp_log_debug("%s:allocate success\n", __func__);
		/*
		 * When the command execution timeout the memory occupied
		 * by sync will be released  in the command execution module,
		 * else the memory will be released after waiting for the
		 * command return normally.
		 */
		goto out;
	}
	q = &cd->queue;
	spin_lock_irqsave(&q->spin_lock, flags);
	smp_wmb(); /* Make sure queue has assigned correctly */
	if (q->cmd_count == q->queue_size) {
		spin_unlock_irqrestore(&q->spin_lock, flags);
		thp_log_err("%s:queue is full\n", __func__);
		WARN_ON(1);
		error = -EIO;
		goto free_sync;
	}
	memcpy(&q->ring_buff[q->wr_index], cmd, sizeof(*cmd));
	q->cmd_count++;
	q->wr_index++;
	q->wr_index %= q->queue_size;
	smp_mb(); /* Make sure queue is refreshed correctly */
	spin_unlock_irqrestore(&q->spin_lock, flags);
	thp_log_debug("%s:%d in ring buff\n", __func__, cmd->command);
	error = NO_ERR;
	wake_up_process(cd->thp_task); /* wakeup thp process */
	if (timeout && (cmd->sync != NULL) &&
		!(wait_for_completion_timeout(&cmd->sync->done, abs(timeout) * HZ))) {
		atomic_set(&cmd->sync->timeout_flag, TS_TIMEOUT);
		thp_log_err("%s:wait cmd respone timeout\n", __func__);
		error = -EBUSY;
		goto out;
	}
	smp_wmb(); /* Make sure code has been completed before function ends */
free_sync:
	kfree(cmd->sync);
	cmd->sync = NULL;
out:
	return error;
}

static int get_one_cmd(struct thp_cmd_node *cmd, u32 panel_id)
{
	unsigned long flags;
	int error = -EIO;
	struct thp_cmd_queue *q = NULL;
	struct thp_core_data *cd = thp_get_core_data(panel_id);

	if (unlikely(!cmd)) {
		thp_log_err("%s:find null pointer\n", __func__);
		goto out;
	}

	q = &cd->queue;
	spin_lock_irqsave(&q->spin_lock, flags);
	smp_wmb(); /* Make sure queue has assigned correctly */
	if (!q->cmd_count) {
		thp_log_debug("%s: queue is empty\n", __func__);
		spin_unlock_irqrestore(&q->spin_lock, flags);
		goto out;
	}
	memcpy(cmd, &q->ring_buff[q->rd_index], sizeof(*cmd));
	q->cmd_count--;
	q->rd_index++;
	q->rd_index %= q->queue_size;
	smp_mb(); /* Make sure queue is refreshed correctly */
	spin_unlock_irqrestore(&q->spin_lock, flags);
	thp_log_debug("%s:%d from ring buff\n", __func__,
		cmd->command);
	error = NO_ERR;
out:
	return error;
}

static bool thp_task_continue(u32 panel_id)
{
	bool task_continue = true;
	unsigned long flags;
	struct thp_core_data *cd = thp_get_core_data(panel_id);

	thp_log_debug("%s:prepare enter idle\n", __func__);
	while (task_continue) {
		if (unlikely(kthread_should_stop())) {
			task_continue = false;
			goto out;
		}
		spin_lock_irqsave(&cd->queue.spin_lock, flags);
		/*
		 * Make sure the memory and assignment are completed
		 * before updating the current process.
		 */
		smp_wmb();
		if (cd->queue.cmd_count) {
			set_current_state(TASK_RUNNING);
			thp_log_debug("%s:TASK_RUNNING\n", __func__);
			goto out_unlock;
		} else {
			set_current_state(TASK_INTERRUPTIBLE);
			thp_log_debug("%s:TASK_INTERRUPTIBLE\n", __func__);
			spin_unlock_irqrestore(&cd->queue.spin_lock, flags);
			schedule();
		}
	}

out_unlock:
	spin_unlock_irqrestore(&cd->queue.spin_lock, flags);
out:
	return task_continue;
}

static int thp_chip_detect_cmd(struct thp_cmd_node *proc_cmd, u32 panel_id)
{
	(void)panel_id;
	thp_chip_detect(proc_cmd);
	return NO_ERR;
}

#if defined CONFIG_LCD_KIT_DRIVER
static int thp_suspend_cmd(struct thp_cmd_node *proc_cmd, u32 panel_id)
{
	(void)proc_cmd;
	thp_suspend_thread(panel_id);
	return NO_ERR;
}

static int thp_resume_cmd(struct thp_cmd_node *proc_cmd, u32 panel_id)
{
	(void)proc_cmd;
	thp_resume_thread(panel_id);
	return NO_ERR;
}

static int thp_multi_resume_cmd(struct thp_cmd_node *proc_cmd, u32 panel_id)
{
	struct thp_core_data *cd = thp_get_core_data(panel_id);

	(void)proc_cmd;
	if (cd->multi_screen_fold == true && cd->dual_thp)
		return NO_ERR;
	thp_multi_resume(panel_id);
	return NO_ERR;
}

static int thp_multi_suspend_cmd(struct thp_cmd_node *proc_cmd, u32 panel_id)
{
	struct thp_core_data *cd = thp_get_core_data(panel_id);

	(void)proc_cmd;
	if (cd->multi_screen_fold == true && cd->dual_thp)
		return NO_ERR;
	thp_multi_suspend(panel_id);
	return NO_ERR;
}

static int tskit_multi_resume_cmd(struct thp_cmd_node *proc_cmd, u32 panel_id)
{
	(void)proc_cmd;
	(void)panel_id;
	tskit_multi_resume();
	return NO_ERR;
}

static int tskit_multi_suspend_cmd(struct thp_cmd_node *proc_cmd, u32 panel_id)
{
	(void)proc_cmd;
	(void)panel_id;
	tskit_multi_suspend();
	return NO_ERR;
}

static int thp_screen_fold_cmd(struct thp_cmd_node *proc_cmd, u32 panel_id)
{
	(void)proc_cmd;
	(void)panel_id;
	return multi_screen_status_switch(SCREEN_FOLDED);
}

static int thp_screen_off_fold_cmd(struct thp_cmd_node *proc_cmd, u32 panel_id)
{
	(void)proc_cmd;
	(void)panel_id;
	return multi_screen_status_switch(SCREEN_OFF_FOLD);
}

static int thp_screen_unfold_cmd(struct thp_cmd_node *proc_cmd, u32 panel_id)
{
	(void)proc_cmd;
	(void)panel_id;
	return multi_screen_status_switch(SCREEN_UNFOLD);
}

static int dual_thp_panel_revert_fold_cmd(struct thp_cmd_node *proc_cmd, u32 panel_id)
{
	(void)proc_cmd;
	(void)panel_id;
	return multi_screen_status_switch_for_multi_thp(SCREEN_FOLDED);
}

static int dual_thp_panel_revert_unfold_cmd(struct thp_cmd_node *proc_cmd, u32 panel_id)
{
	(void)proc_cmd;
	(void)panel_id;
	return multi_screen_status_switch_for_multi_thp(SCREEN_UNFOLD);
}
#endif

struct thp_proc_cmd_group g_thp_proc_cmd_table[] = {
	{ TS_CHIP_DETECT, thp_chip_detect_cmd },
#if defined CONFIG_LCD_KIT_DRIVER
	{ TS_SUSPEND, thp_suspend_cmd },
	{ TS_RESUME, thp_resume_cmd },
	{ THP_MUTIL_RESUME_THREAD, thp_multi_resume_cmd },
	{ THP_MUTIL_SUSPEND_THREAD, thp_multi_suspend_cmd },
	{ TSKIT_MUTIL_RESUME_THREAD, tskit_multi_resume_cmd },
	{ TSKIT_MUTIL_SUSPEND_THREAD, tskit_multi_suspend_cmd },
	{ TS_SCREEN_FOLD, thp_screen_fold_cmd },
	{ TS_SCREEN_OFF_FOLD, thp_screen_off_fold_cmd },
	{ TS_SCREEN_UNFOLD, thp_screen_unfold_cmd },
	{ DUAL_THP_PANEL_REVERT_FOLD, dual_thp_panel_revert_fold_cmd },
	{ DUAL_THP_PANEL_REVERT_UNFOLD, dual_thp_panel_revert_unfold_cmd },
#endif
};

static int thp_proc_command(struct thp_cmd_node *cmd)
{
	int error = NO_ERR;
	struct thp_cmd_sync *sync = NULL;
	struct thp_cmd_node *proc_cmd = cmd;
	struct thp_cmd_node *out_cmd = NULL;
	int i;

	if (!cmd) {
		thp_log_err("%s:invalid cmd\n", __func__);
		goto out;
	}
	sync = cmd->sync;
	out_cmd = &thp_get_core_data(cmd->cmd_param.thread_id)->pang_cmd_buff;
	/* discard timeout cmd */
	if (sync && (atomic_read(&sync->timeout_flag) == TS_TIMEOUT)) {
		kfree(sync);
		goto out;
	}
	while (true) {
		out_cmd->command = TS_INVAILD_CMD;
		for (i = 0; i < ARRAY_SIZE(g_thp_proc_cmd_table); ++i)
			if (proc_cmd->command == g_thp_proc_cmd_table[i].cmd) {
				error = g_thp_proc_cmd_table[i].cmd_func(proc_cmd, cmd->cmd_param.panel_id);
				break;
			}

		thp_log_debug("%s:command :%d process result:%d\n",
			__func__, proc_cmd->command, error);
		if (out_cmd->command != TS_INVAILD_CMD) {
			thp_log_debug("%s:related command :%d  need process\n",
				__func__, out_cmd->command);
			/* ping - pang */
			swap(proc_cmd, out_cmd);
		} else {
			break;
		}
	}
	/* notify wait threads by completion */
	if (sync) {
		smp_mb(); /* Make sure current timeout_flag is up to date */
		thp_log_debug("%s:wakeup threads in waitqueue\n", __func__);
		if (atomic_read(&sync->timeout_flag) == TS_TIMEOUT)
			kfree(sync);
		else
			complete(&sync->done);
	}

out:
	return error;
}

static int thp_thread(void *p)
{
	struct thp_core_data *cd = (struct thp_core_data *)p;
	static const struct sched_param param = {
		/* The priority of thread scheduling is 99 */
		.sched_priority = 99,
	};
	/*
	 * Make sure buff is properly refreshed
	 * before the process is executed.
	 */
	smp_wmb();
	thp_log_info("%s: in\n", __func__);
	memset(&cd->ping_cmd_buff, 0, sizeof(cd->ping_cmd_buff));
	memset(&cd->pang_cmd_buff, 0, sizeof(cd->pang_cmd_buff));
	/*
	 * Make sure buff is properly refreshed
	 * before the process is executed.
	 */
	smp_mb();
	sched_setscheduler(current, SCHED_RR, &param);
	while (thp_task_continue(cd->panel_id)) {
		/* get one command */
		while (!get_one_cmd(&cd->ping_cmd_buff, cd->panel_id)) {
			thp_proc_command(&cd->ping_cmd_buff);
			memset(&cd->ping_cmd_buff, 0, sizeof(cd->ping_cmd_buff));
			memset(&cd->pang_cmd_buff, 0, sizeof(cd->pang_cmd_buff));
		}
	}
	thp_log_err("%s: stop\n", __func__);
	return NO_ERR;
}

int thp_thread_init(struct thp_core_data *cd, u32 panel_id)
{
    if (!cd) {
        thp_log_err("%s: nullprt\n", __func__);
        return -EINVAL;
    }
	cd->thp_task = kthread_create(thp_thread, cd, "ts_thread:%u", panel_id);
	if (IS_ERR(cd->thp_task)) {
		thp_log_err("%s: creat thread failed\n", __func__);
		return -ENODEV;
	}
	cd->queue.rd_index = 0;
	cd->queue.wr_index = 0;
	cd->queue.cmd_count = 0;
	cd->queue.queue_size = THP_CMD_QUEUE_SIZE;
	spin_lock_init(&cd->queue.spin_lock);
	smp_mb(); /* Make sure queue is initialized before wake up the task */
	wake_up_process(cd->thp_task);
	return 0;
}